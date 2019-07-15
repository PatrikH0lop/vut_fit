<?php
session_start();
include "../timeout.php";
check_timeout();
?>

<html>
<header><title>IIS Project 2018</title></header>

<head>
<link rel="stylesheet" href="../css/style.css">
</head>

<body>

<div id="PortalPage">
<h1>Customer portal</h1>
<?php
    $db = mysqli_init();
    if (!(mysqli_real_connect($db, 'localhost', 'REDACTED', 'REDACTED', 'REDACTED', 0, '/var/run/mysql/mysql.sock')))
    {
        die('cannot connect '.mysqli_connecterror());
    }

	//session_start();
	if (isset($_SESSION["username"]) && isset($_SESSION["role"])
		&& $_SESSION["role"] == "customer")
	{
		echo "<h3>Logged as: ";
		echo  $_SESSION["username"];
		echo "</h3>";
		$query = "SELECT id FROM Osoba WHERE login='{$_SESSION["username"]}'";
		$result = mysqli_query($db, $query);
		$str = $result->fetch_row();
		$_SESSION['user_id'] = $str[0];
	}
	else
	{
		header("Location: ../index.php");
	}
?>
<p><a href="../index.php">Logout</a> <a href="transaction_history.php">Show transaction history</a></p>
</div>

<div id="PortalPage">

<h3>Customer options</h3>
<p> Please fill the form to add new tea to chart </p> <br>
<form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
	<?php	
	
	if (isset($_POST["action"]) && $_POST["action"] == "New Order") {
		unset($_SESSION["obj_id"]);
	} else if (isset($_POST["action"]) && $_POST["action"] == "Add to Order") {
		echo "<meta http-equiv='refresh' content='0'>";
	}
	
	if (!isset($_SESSION["obj_id"])) {
		$_SESSION["new_obj"] = "new";
	} else {
		echo "Current Order ID: {$_SESSION["obj_id"]} <br><br>";
	}

	?>
	<input type="submit" name="action" value="New Order"/>

	Tea* <select name = "tea" id = "form-tea" onchange="select_brews()"/>
		<?php
			$sql1 = "SELECT nazev, id FROM Caj;";
			$result = mysqli_query($db, $sql1);
			while ($row = $result->fetch_row()) {
				echo "<option data-id=$row[1] value=$row[0]>$row[0] (ID $row[1])</option>";
			}
		?>
	</select><br>
	Brew* <select name = "brew" id = "form-brews"/>
		<?php
			$sql2 = "SELECT id, mnozstvi, cena_100g, id_caj, sleva FROM Varka;";
			$result = mysqli_query($db, $sql2);
			while ($row = $result->fetch_row()) {
				$new_price = $row[2] * (1 - ($row[4] / 100));
				if ($row[4] == 0) {
					echo "<option name='tea_brew_option' value=$row[0] data-caj=$row[3]>Amount: $row[1], Cost/100g: $row[2] </option>";
				} else {
					echo "<option name='tea_brew_option' value=$row[0] data-caj=$row[3]>Amount: $row[1], Cost/100g: $new_price &nbsp;&nbsp;&nbsp;&nbsp; SALE $row[4]%! Original price: $row[2] </option>";
				}
			}
		?>
	</select><br>
	Amount*: <input type="text" id="amount" name="amount"/> <br>
	<input type="submit" name="action" value="Add to Order"/>
</form>

<?php
	if (isset($_POST["action"]) && $_POST["action"] == "Add to Order") {
	if (isset($_POST["amount"]) && isset($_POST["tea"]) && isset($_POST["brew"])) {
		$amount = $_POST["amount"];
		$tea = $_POST["tea"];
		$brew = $_POST["brew"];
		
		$get_cost = "SELECT cena_100g, mnozstvi, sleva FROM Varka WHERE id={$brew};";
		if (!($result = mysqli_query($db, $get_cost))) {
			echo mysqli_error($db);
		}
		$cs = $result->fetch_row();
		$cost = $cs[0];
		$max_amount = $cs[1];
		$discount = $cs[2];
		
		if ($amount === "" || !(is_numeric($amount))) {
			echo '<script>alert("Please fill the amount correctly");</script>';
		} else if ($amount > $max_amount) {
			echo '<script>alert("Please lower the amount!");</script>';
		} else if ($amount <= 0) {
			echo '<script>alert("Please increase the amount!");</script>';
		}

		//order does not exist
		else if ($_SESSION["new_obj"] === "new") {
			
			// Calculate the initial cost and create bill
			$total_cost = $cost * $amount * (1 - $discount / 100);
			$create_bill = "INSERT INTO Faktura (cena) VALUES " . "({$total_cost});";
			if (!mysqli_query($db, $create_bill)) {
				echo mysqli_error($db);
			}
			$bill_id = mysqli_insert_id($db);

			// Create Objednavka
			$create_order = "INSERT INTO Objednavka (stav, datum_vytvoreni, id_faktura, id_odberatel) VALUES " . "(\"new\", now(), {$bill_id}, {$_SESSION['user_id']});";
			if (!mysqli_query($db, $create_order)) {
				echo mysqli_error($db);
			};
			$_SESSION["obj_id"] = mysqli_insert_id($db);
			
			// Calculate the cost and add chart data
			$add_to_chart = "INSERT INTO Obsahuje (mnozstvi, cena, id_objednavka, id_varka) VALUES " . "({$amount}, {$total_cost}, {$_SESSION["obj_id"]}, {$brew});";
			if (!mysqli_query($db, $add_to_chart)) {
				echo mysqli_error($db);
			};

			// Change the amount for distribution
			$new_amount = $max_amount - $amount;
			$update_amount = "UPDATE Varka SET mnozstvi=$new_amount WHERE id=$brew";
			if (!mysqli_query($db, $update_amount)) {
				echo mysqli_error($db);
			}
			
			// set order as active
			$_SESSION["new_obj"] = "active";
		
		// Creating order is active
		} else if ($_SESSION["new_obj"] === "active") {

			// Calculate the cost and update bill 
			// Find bill id
			$total_cost = $cost * $amount * (1 - $discount / 100);
			$get_bill_id = "SELECT id_faktura FROM Objednavka WHERE id={$_SESSION["obj_id"]};";
			$result = mysqli_query($db, $get_bill_id);
			$row = $result->fetch_row();
			$bill_id = $row[0];
			// update the bill
			$update_bill = "UPDATE Faktura SET cena = cena + $total_cost WHERE id=$bill_id;";
			if (!mysqli_query($db, $update_bill)) {
				echo mysqli_error($db);
			}

			// Add chart data
			$add_to_chart = "INSERT INTO Obsahuje (mnozstvi, cena, id_objednavka, id_varka) VALUES " . "({$amount}, {$total_cost}, {$_SESSION["obj_id"]}, {$brew});";
			if (!mysqli_query($db, $add_to_chart)) {
				echo mysqli_error($db);
			}
			
			// Update storage amount
			$new_amount = $max_amount - $amount;
			$update_amount = "UPDATE Varka SET mnozstvi=$new_amount WHERE id=$brew";
			if (!mysqli_query($db, $update_amount)) {
				echo mysqli_error($db);
			}
		}
	} else {
		echo "<p>required field not set</p>";
	}
	}
?>

Items currently ordered
<table>
<tr> <th>Order</th><th>Tea</th><th>Amount</th> <th>Price</th><th>Date</th></tr>
<?php
if (isset($_SESSION["obj_id"])) {
	$find_order = "SELECT Objednavka.id, Obsahuje.mnozstvi, cena, datum_vytvoreni, Caj.nazev FROM Objednavka INNER JOIN Obsahuje ON Obsahuje.id_objednavka=Objednavka.id INNER JOIN Varka ON Varka.id=Obsahuje.id_varka INNER JOIN Caj ON Caj.id=Varka.id_caj WHERE id_odberatel={$_SESSION['user_id']} AND Objednavka.id={$_SESSION["obj_id"]};";
	if (!($result = mysqli_query($db, $find_order))) {
		echo mysqli_error($db);
	};
	while ($row = $result->fetch_row()) {
		echo "<tr><td>{$row[0]}</td><td>$row[4]</td><td>{$row[1]}</td><td>{$row[2]}</td><td>{$row[3]}</td></tr>";
	}
}
?>
</table>

</div>

<div id="PortalPageRightWrapper">
<div id="PortalPageRight">
<h3>Tea offers</h3>

	<!-- Generate tea offers from DB -->
	<?php

	$tea_query = "SELECT nazev, druh, dodavatel, kvalita, zeme_puvodu, nazev_oblasti FROM Caj;";
	$hint_string = "";
	$brew_string = "";

	$index = 0;

    $result = mysqli_query($db, $tea_query);
	if ($result !== False) {
		while ($row = $result->fetch_row()) {
			echo "<div id=\"TeaOffer_{$index}\"" . " name='TeaOffer'>";
			echo "Name: {$row[0]}<br>";
			$hint_string .= "<div id=\"popup_{$index}\" class=\"reveal-modal\">";
			$hint_string .= "<h2>Tea: {$row[0]}</h2>";
			//$hint_string .= '<p> The best tea harvested in Africa by our happy employees</p>';
			$hint_string .= '<img alt="Tea" align="left" src="../images/tea/' . $row[1] .'" height=100 width=100>';
			$hint_string .= "&nbsp<b>Type</b>: {$row[1]}<br>&nbsp;";
			$hint_string .= "<b>Supplier:</b> {$row[2]}<br>&nbsp;";
			$hint_string .= "<b>Quality:</b> {$row[3]}<br>&nbsp;";
			$hint_string .= "<b>Country of origin:</b> {$row[4]}<br>&nbsp;";
			$hint_string .= "<b>Province:</b> {$row[5]}<br>";
			$hint_string .= '</div>';

			echo '</div>';
			echo '<br>';
			$index += 1;
		}
	}
	
	echo "</div>";
	echo "</div>";

	echo $hint_string;
	?>


<script>
	// tea offer popups listeners
	document.getElementsByName("TeaOffer").forEach(function(elem) {
		elem.addEventListener("mouseover", function() {
			document.getElementById("popup_" + elem.getAttribute("id").substr(9)).style.display = 'block';
		});
	});
	document.getElementsByName("TeaOffer").forEach(function(elem) {
		elem.addEventListener("mouseleave", function() {
			document.getElementById("popup_" + elem.getAttribute("id").substr(9)).style.display = 'none';
		});
	});
</script>

<script>
	// show brews only for selected tea
	var select = document.createElement("select");
	var original_form = document.getElementById("form-brews");
	for (var i=0; i<original_form.length; i++) {
		var option = document.createElement("option");
		option.text = original_form[i].text;
		option.setAttribute("value", original_form[i].getAttribute("value"));
		option.setAttribute("data-caj", original_form[i].getAttribute("data-caj"));
		option.setAttribute("name", "tea_brew_option");
		select.add(option);
	}

	function select_brews() {
		var dropdown = document.getElementById("form-tea");
		var option = dropdown.options[dropdown.selectedIndex].getAttribute("data-id");
		//alert(option)

		var brews = document.getElementById("form-brews");
		brews.innerHTML = "";

		for (var i=0; i<select.length; i++) {
			//alert(select[i].getAttribute("data-caj") + " " + option);
			if (select[i].getAttribute("data-caj") === option) {
				var opt = document.createElement("option");
				opt.text = select[i].text;	
				opt.setAttribute("value", select[i].getAttribute("value"));
				opt.setAttribute("data-caj", select[i].getAttribute("data-caj"));
				opt.setAttribute("name", "tea_brew_option");
				brews.add(opt);
			}
		}
		document.getElementById("form-brews").selectedIndex = "0";
	}
	select_brews();
</script>

</body>
</html>


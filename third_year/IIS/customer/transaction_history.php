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

<?php
$db = mysqli_init();
if (!(mysqli_real_connect($db, 'localhost', 'REDACTED', 'REDACTED', 'REDACTED', 0, '/var/run/mysql/mysql.sock'))) 
{
        die('cannot connect '.mysqli_connecterror());
}
?>

<script>
	function print_bill() {
		var bill = document.getElementById("bill_to_print").innerHTML;
		var page = document.body.innerHTML;

		var print_window = window.open('', 'Print', 'height=700, width=900');
		if (print_window == null) {
			alert("Print popup window was blocked");
		} else {
			print_window.document.write('<html><head><title>Print window</title></head><body>');
			print_window.document.write(bill);
			print_window.document.write('</body></html>');
			print_window.document.close();
			print_window.focus();
			print_window.print();
			print_window.close();
		}
	}
</script>

<body>

<div id="PortalPage">
<h2>Billing history</h2>

<?php
    //session_start();
    if (isset($_SESSION["user_id"]))
    {
           echo "<h3>Logged as: ";
           echo  $_SESSION["username"];
           echo "</h3>";
    }
    else
    {
           // TODO: Transfer back to login page (uncomment once work is done)
           header("Location: ../index.php");
    }
	echo '<p><a href="index.php">Back to customer portal</a></p>';
	
	// get billing data
	$billing_history = "SELECT Objednavka.id, stav, datum_vytvoreni, cena FROM Objednavka INNER JOIN Faktura ON Faktura.id=Objednavka.id_faktura WHERE Objednavka.id_odberatel=\"{$_SESSION["user_id"]}\" ORDER BY Objednavka.id DESC;";
	$results = mysqli_query($db, $billing_history);
	if (!$results) {
		echo mysqli_error($db);
	}
	
	// print table
	echo "<table align='center'><tr><th>Order (id)</th><th>State</th><th>Price</th><th>Date</th></tr>";
	while ($row = $results->fetch_row()) {
		echo "<tr><td>$row[0]</td><td>$row[1]</td><td>$row[3]</td><td>$row[2]</td></tr>";
	}
	echo "</table>";
?>
</div>

<div id="PortalPage">
<h2>Details</h2>

<table>
<tr> <th>Order</th><th>Tea</th><th>Amount</th> <th>Price</th><th>Date</th></tr>

<?php

$find_order = "SELECT Objednavka.id, Obsahuje.mnozstvi, cena, datum_vytvoreni, Caj.nazev FROM Objednavka INNER JOIN Obsahuje ON Obsahuje.id_objednavka=Objednavka.id INNER JOIN Varka ON Varka.id=Obsahuje.id_varka INNER JOIN Caj ON Caj.id=Varka.id_caj WHERE id_odberatel={$_SESSION['user_id']} ORDER BY Objednavka.id DESC;";

if (!($result = mysqli_query($db, $find_order))) {
	echo mysqli_error($db);
};

while ($row = $result->fetch_row()) {
	echo "<tr><td>{$row[0]}</td><td>$row[4]</td><td>{$row[1]}</td><td>{$row[2]}</td><td>{$row[3]}</td></tr>";
}

?>

</table>
	
</div>

<div id="PortalPageRightWrapper">
<div id="PortalPageRight">

<h2>Print your bills</h2>

<form action="<?=$_SERVER["PHP_SELF"]?>" method="post">

Order ID*<br>
<input type="text" id="order_id" name="order_id"> </input>
<input type="submit" value="Print your bill" </input>
</form>

<?php
if (isset($_POST["order_id"])) {
		$find_order = "SELECT Osoba.jmeno, Osoba.adresa, Osoba.kontakt FROM Objednavka INNER JOIN Odberatel ON Objednavka.id_odberatel=Odberatel.id INNER JOIN Osoba ON Odberatel.id=Osoba.id WHERE Objednavka.id={$_POST["order_id"]} and Objednavka.id_odberatel={$_SESSION['user_id']}";
		$results = mysqli_query($db, $find_order);
		if (!$results || $results->num_rows == 0) {
			echo "Order not found";
		} else {
			$row = $results->fetch_row();
			echo "
			<div id='bill_to_print'> 
				<b>Personal information</b><br>
				Order ID: {$_POST["order_id"]} <br>
				Name: $row[0] <br>
				Address: $row[1] <br>
				Contact: $row[2] <br> <br>
				<b>Order details</b><br>
			";

			$find_order = "SELECT Obsahuje.mnozstvi, cena, datum_vytvoreni, Caj.nazev FROM Objednavka INNER JOIN Obsahuje ON Obsahuje.id_objednavka=Objednavka.id INNER JOIN Varka ON Varka.id=Obsahuje.id_varka INNER JOIN Caj ON Caj.id=Varka.id_caj WHERE id_odberatel={$_SESSION['user_id']} and Objednavka.id={$_POST["order_id"]};";

			if (!($result = mysqli_query($db, $find_order))) {
				echo mysqli_error($db);
			};

			echo "<table><tr><th>Tea</th><th>Amount</th> <th>Price</th><th>Date</th></tr>";
			while ($row_2 = $result->fetch_row()) {
				echo "<tr><td>$row_2[3]</td><td>{$row_2[0]}</td><td>{$row_2[1]}</td><td>{$row_2[2]}</td></tr>";
			}
			echo "</table>";

			$select_bill = "SELECT Faktura.id, cena FROM Faktura INNER JOIN Objednavka ON Objednavka.id_faktura=Faktura.id WHERE Objednavka.id={$_POST["order_id"]}";
			if (!($result = mysqli_query($db, $select_bill))) {
				echo mysqli_error($db);
			}
			$result_3 = $result->fetch_row();
			echo "<br><b>Bill information</b><br>";
			echo "Bill ID: $result_3[0]<br>";
			echo "<b>Total cost</b>: $result_3[1] <br></div>";
			echo "<script> print_bill(); </script>";
		}
}
?>
</div>
</div>

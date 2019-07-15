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

<body>

<div id="PortalPage">
<h1>Order Options</h1>
<?php
	//session_start();
	if (isset($_SESSION["username"]) && isset($_SESSION["role"]) && 
		($_SESSION["role"] == "admin" || $_SESSION["role"] == "emloyee"))
	{
		echo "<h3>Logged as: ";
		echo  $_SESSION["username"];
		echo "</h3>";
	}
	else
	{
		header("Location: ../index.php");
	}
?>
<p><a href="../index.php">Back to login</a>
	&nbsp&nbsp&nbsp
	<a href="./index.php">Back to portal</a></p>
</div>

<div id="PortalPage">
<h3>Change order status</h3>
<form action="<?=$_SERVER["PHP_SELF"]?>"  method="post">
	Choose Order:
	<select name="country">
		<option hidden disabled selected value> -- select order -- </option>
		<?php
			$sql1 = "SELECT Objednavka.id, login, stav FROM Objednavka JOIN
					Odberatel on Objednavka.id_odberatel = Odberatel.id JOIN
					Osoba on Odberatel.id = Osoba.id;";	
			$result1 = mysqli_query($db, $sql1);
			while ($row = $result1->fetch_row())
			{
				echo "<option value = $row[0]>";
				echo "$row[0] - $row[1] - $row[2]";
				echo "</option>";
			} 
		?>
	</select><br>
	New Status:
	<select name="type">
		<option value = "done">Done</option>
		<option value = "pending">Pending</option>
		<option value = "canceled">Canceled</option>
	</select><br>

	<input type="submit" name="change" value="Confirm" />
</form>
<?php
// Change selected order status to new one
if (isset($_POST["change"]))
{
	if (!isset($_POST["type"]) || !isset($_POST["country"]))
	{
		echo "<p>Please provide Order ID and new status</p>";
	}
	else
	{
		$id = $_POST["country"];
		$status = $_POST["type"];
		$sql2 = "UPDATE Objednavka SET stav = '$status' WHERE id = $id;";
		mysqli_query($db, $sql2);
		echo "<p>Updated</p>";
	}
}
?>
</div>

<div id="PortalPageRightWrapper">
<div id="PortalPageRight">
<h3>Order Database</h3>

<?php
echo "<table>";
	echo "<tr>";
		echo "<th> Order ID </th>";
		echo "<th> Status </th>";
		echo "<th> Date </th>";
		echo "<th> Cost </th>";
		echo "<th> Customer </th>";
	echo "</tr>";

$sql3 = "SELECT Objednavka.id, stav, datum_vytvoreni, cena, login 
		FROM Objednavka JOIN 
		Faktura on Objednavka.id_faktura = Faktura.id JOIN
		Odberatel on Objednavka.id_odberatel = Odberatel.id JOIN
		Osoba on Osoba.id = Odberatel.id;";
$result3 = mysqli_query($db, $sql3);
while ($row = $result3->fetch_row()) 
{
	printf("<tr>\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n</tr>\n",
			$row[0], $row[1], $row[2], $row[3], $row[4]);
}
echo "</table>";
?>


</div>

</div>
</body>



</html>




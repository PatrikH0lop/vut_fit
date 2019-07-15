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
<h1>Discounts</h1>
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
<h3>Set new discount</h3>
<p>Please choose batch and new discount</p>
<form action="<?=$_SERVER["PHP_SELF"]?>"  method="post">
Batch: <select name="province">
<option hidden disabled selected value> -- select Batch -- </option>
<?php
$sql1 = "SELECT Varka.id, Caj.id, nazev, dodavatel, 
		zeme_puvodu, mnozstvi, cena_100g, sleva
		FROM Varka JOIN Caj on Varka.id_caj = Caj.id;";
$result = mysqli_query($db, $sql1);
while ($row = $result->fetch_row())
{
	echo "<option value=$row[0]>$row[0] : $row[2] - $row[7]%</option>";
}
?>
</select><br>
New discount (in %): <input type="number" id="amount" name="amount" value="0" /><br>
<input type="submit" name="disc" value="Confirm" />
</form>

<?php
if (isset($_POST["disc"]))
{
	if (isset($_POST["province"]) && isset($_POST["amount"]))
	{
		$disc = $_POST["amount"];
		$batch = $_POST["province"];
		if ($disc > 100 || $disc < 0)
			echo "<p>Please provide a valid number</p>";
		else
		{
			$sql2 = "UPDATE Varka SET sleva = $disc WHERE id = $batch;";
			mysqli_query($db, $sql2);
			echo "<p>Updated</p>";
		}
	}
	else
	{
		echo "<p>Please choose a batch and new discount</p>";
	}
}
?>
</div>


<div id="PortalPageRightWrapper">
<div id="PortalPageRight">
<h3>Batch Database</h3>

<?php
echo "<table>";
	echo "<tr>";
		echo "<th> Batch ID </th>";
		echo "<th> Tea ID </th>";
		echo "<th> Tea Name </th>";
		echo "<th> Distributor </th>";
		echo "<th> Country </th>";
		echo "<th> In stock </th>";
		echo "<th> Original price </th>";
		echo "<th> Discount </th>";
	echo "</tr>";

$result = mysqli_query($db, $sql1);
while ($row = $result->fetch_row()) 
{
	printf("<tr>\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n<th>%s%%</th>\n</tr>\n",
			$row[0], $row[1], $row[2], $row[3], $row[4], $row[5], $row[6], $row[7]);
}
echo "</table>";
?>


</div>

</div>
</body>



</html>




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
<h1>Batch Options</h1>
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
<h3>Add Batch</h3>
<p>Please fill the form to add new batch (* = required field)</p>
<!-- Batch form -->
<form action="<?=$_SERVER["PHP_SELF"]?>"  method="post">
        Contains tea*: <select name="name" />
				<option hidden disabled selected value> -- select Tea -- </option>
				<?php
				$sql2 = "SELECT id, nazev FROM Caj;";
				$result = mysqli_query($db, $sql2);
				while ($row = $result->fetch_row())
				{
					echo "<option value=$row[0]>$row[0]: $row[1]</option>";
				}
				?>
			</select><br>
        Price per 100g*: <input type="number" id="price" name="price" value="<?php echo isset($_POST["price"]) ? $_POST["price"] : 0 ?>" />
        Amount*: <input type="number" id="amount" name="amount" value="<?php echo isset($_POST["amount"]) ? $_POST["amount"] : 0 ?>" /><br>
		
        <input type="submit" name="add" value="Confirm" />
</form>
<?php
// Remove chosen batch
if (isset($_POST["add"]))
{
	if (isset($_POST["name"]) && isset($_POST["price"]) && isset($_POST["amount"])
		&& $_POST["price"] != 0 && $_POST["amount"] != 0 && $_POST["name"] != "")
	{
		$tea = $_POST["name"];
		$price = $_POST["price"];
		$stock = $_POST["amount"];
		$sql3 = "INSERT INTO Varka 
				(mnozstvi, cena_100g, id_caj)
				VALUES($stock, $price, '$tea');";
		mysqli_query($db, $sql3);
	}
	else
	{
		echo "<p>Please fill all required windows (marked *)</p>";
	}
}
?>
	
</div>


<div id="PortalPage">
<h3>Remove Batch</h3>
<p>Please choose batch to be removed</p>
<!-- Batch removing form -->
<form action="<?=$_SERVER["PHP_SELF"]?>"  method="post">
<select name="Teas">
<option hidden disabled selected value> -- select Batch -- </option>
<?php
$sql1 = "SELECT Varka.id, Caj.id, nazev, dodavatel, 
		zeme_puvodu, mnozstvi, cena_100g, sleva
		FROM Varka JOIN Caj on Varka.id_caj = Caj.id;";
$result = mysqli_query($db, $sql1);
while ($row = $result->fetch_row())
{
	echo "<option value=$row[0]>$row[0] : $row[2]</option>";
}
?>

</select>
<input type="submit" name="remove" value="Confirm" />
</form>

<?php
// Remove chosen tea
if (isset($_POST["remove"]))
{
	$selected_batch = $_POST["Teas"];
	$sql = "DELETE FROM Varka WHERE id=$selected_batch";
	if (mysqli_query($db, $sql))
	{
		header("Location: ./batch_options.php");
	}
	else
	{
		echo "This Batch cannot be deleted, it is used in existing order";
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




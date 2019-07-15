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
<h1>Country Options</h1>
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
<h3>Add Country</h3>
<p>Please fill the form to add new Country (* = required field)</p>
<!-- Country form -->
<form action="<?=$_SERVER["PHP_SELF"]?>"  method="post">
        Country*: <input type="text" id="name" name="name" value="<?php echo isset($_POST["name"]) ? htmlspecialchars($_POST["name"]) : "" ?>" /> <br>
        Continent*:
				<select name="province" />
				<option value = "Asia">Asia</option>	
				<option value = "Africa">Africa</option>	
				<option value = "Europe">Europe</option>	
				<option value = "South America">South America</option>	
				<option value = "North America">North America</option>	
			</select><br>
        <input type="submit" name="add" value="Confirm" />
</form>
<?php
if (isset($_POST["add"]))
{
	if ((isset($_POST["name"])) && (isset($_POST["province"])) &&
		$_POST["name"] != "" && strlen($_POST["name"]) <= 25 && 
		strlen($_POST["province"]) <= 20)
	{
		$name = $_POST["name"];
		$continent = $_POST["province"];

		$sql3 = "INSERT INTO Zeme 
				(nazev_zeme, kontinent) VALUES('$name', '$continent');";
		mysqli_query($db, $sql3);
	}
	else
	{
		if (strlen($_POST["name"]) > 25)
			echo "<p>Country name cannot be longer that 25 characters</p>";
		else	
			echo "<p>Please fill all required windows (marked *)</p>";
	}
}
?>
	
</div>


<div id="PortalPage">
<h3>Remove Country</h3>
<p>Please choose Country to be removed</p>
<!-- Country removing form -->
<form action="<?=$_SERVER["PHP_SELF"]?>"  method="post">
<select name="Country">
<option hidden disabled selected value> -- select Country -- </option>
<?php
$sql1 = "SELECT nazev_zeme, kontinent FROM Zeme;";
$result = mysqli_query($db, $sql1);
while ($row = $result->fetch_row())
{
	echo "<option value=$row[0]>$row[0] ($row[1])</option>";
}
?>

</select>
<input type="submit" name="remove" value="Confirm" />
</form>

<?php
// Remove chosen country
if (isset($_POST["remove"]))
{
	$selected_country = $_POST["Country"];
	$sql = "DELETE FROM Zeme WHERE nazev_zeme='$selected_country'";
	if (mysqli_query($db, $sql))
	{
		header("Location: ./country_options.php");
	}
	else
	{
		echo "<p>Country cannot be deleted, it is used in some tea batch(es)</p>";
	}
}
?>
</div>


<div id="PortalPageRightWrapper">
<div id="PortalPageRight">
<h3>Country Database</h3>

<?php
echo "<table>";
	echo "<tr>";
		echo "<th> Name </th>";
		echo "<th> Continent </th>";
	echo "</tr>";

$result = mysqli_query($db, $sql1);
while ($row = $result->fetch_row()) 
{
	printf("<tr>\n<th>%s</th>\n<th>%s</th>\n</tr>\n",
			$row[0], $row[1]);
}
echo "</table>";
?>


</div>

</div>
</body>



</html>




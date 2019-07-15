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
<h1>Province Options</h1>
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
<h3>Add Province</h3>
<p>Please fill the form to add new Province (* = required field)</p>
<!-- Province form -->
<form action="<?=$_SERVER["PHP_SELF"]?>"  method="post">
        Province*: <input type="text" id="name" name="name"/> <br>
        Characteristics:<br><input type="text" id="char" name="char" value="<?php echo isset($_POST["char"]) ? htmlspecialchars($_POST["char"]) : "" ?>"/> <br>
		Desription:<br> <input type="text" id="desc" name="desc" value="<?php echo isset($_POST["desc"]) ? htmlspecialchars($_POST["desc"]) : "" ?>" /> <br>
        <input type="submit" name="add" value="Confirm" />
</form>
<?php
if (isset($_POST["add"]))
{
	if ((isset($_POST["name"])) && strlen($_POST["char"]) <= 25 &&
		strlen($_POST["char"]) <= 50 && strlen($_POST["desc"] <= 100) &&
		$_POST["name"] != "")
	{
		$name = $_POST["name"];
		$char = $_POST["char"];
		$desc = $_POST["desc"];

		if ($char == "")
			$char = "NULL";
		else
			$char = "'$char'";
		if ($desc == "")
			$desc = "NULL";
		else
			$desc = "'$desc'";

		$sql3 = "INSERT INTO Oblast 
				(nazev_oblast, charakteristika, popis) 
				VALUES('$name', $char, $desc);";
		mysqli_query($db, $sql3);
		echo mysqli_error($db);
	}
	else
	{
		if (strlen($_POST["name"]) > 25)
			echo "<p>Province name cannot be longer that 25 characters</p>";
		else if (strlen($_POST["desc"]) > 100)
			echo "<p>Province description cannot be longer that 25 characters</p>";
		else if (strlen($_POST["char"]) > 50)
			echo "<p>Province characteristics cannot be longer that 25 characters</p>";
		else	
			echo "<p>Please fill all required windows (marked *)</p>";
	}
}
?>
	
</div>


<div id="PortalPage">
<h3>Remove Province</h3>
<p>Please choose Province to be removed</p>
<!-- Province removing form -->
<form action="<?=$_SERVER["PHP_SELF"]?>"  method="post">
<select name="Country">
<option hidden disabled selected value> -- select Province -- </option>
<?php
$sql1 = "SELECT nazev_oblast, charakteristika, popis FROM Oblast;";
$result = mysqli_query($db, $sql1);
while ($row = $result->fetch_row())
{
	echo "<option value=$row[0]>$row[0]</option>";
}
?>

</select>
<input type="submit" name="remove" value="Confirm" />
</form>

<?php
// Remove chosen province
if (isset($_POST["remove"]))
{
	$selected_province = $_POST["Country"];
	$sql = "DELETE FROM Oblast WHERE nazev_oblast='$selected_province'";
	if (mysqli_query($db, $sql))
	{
		header("Location: ./province_options.php");
	}
	else
	{
		echo "<p>Province cannot be deleted, it is used in some tea batch(es)</p>";
	}
}
?>
</div>


<div id="PortalPageRightWrapper">
<div id="PortalPageRight">
<h3>Province Database</h3>

<?php
echo "<table>";
	echo "<tr>";
		echo "<th> Name </th>";
		echo "<th> Characteristics </th>";
		echo "<th> Desription </th>";
	echo "</tr>";

$result = mysqli_query($db, $sql1);
while ($row = $result->fetch_row()) 
{
	printf("<tr>\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n</tr>\n",
			$row[0], $row[1], $row[2]);
}
echo "</table>";
?>


</div>

</div>
</body>



</html>




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
<h1>Tea Options</h1>
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
<h3>Add Tea</h3>
<p>Please fill the form to add new tea (* = required field)</p>
<!-- Tea form -->
<form action="<?=$_SERVER["PHP_SELF"]?>"  method="post">
        Tea name*: <input type="text" id="name" name="name" value="<?php echo isset($_POST["name"]) ? htmlspecialchars($_POST["name"]) : "" ?>"/> <br>
        Distributor*: <input type="text" id="distributor" name="distributor" value="<?php echo isset($_POST["distributor"]) ? htmlspecialchars($_POST["distributor"]) : "" ?>"/><br>
        Type*:&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp    
			<select name="type" />
				<option value = "Green">Green</option>	
				<option value = "Black">Black</option>	
				<option value = "Herb">Herb</option>	
				<option value = "Pu-Erh">Pu-Erh</option>	
				<option value = "Oolong">Oolong</option>	
				<option value = "White">White</option>	
			</select>&nbsp&nbsp&nbsp&nbsp&nbsp
        Brew time (minutes): <input type="number" id="brew" name="brew" value="<?php echo isset($_POST["brew"]) ? $_POST["brew"] : 0 ?>"/><br>
		Origin Country*:
			<select name="country" />
				<option hidden disabled selected value> -- select Country -- </option>
				<?php
				$sql2 = "SELECT nazev_zeme FROM Zeme;";
				$result = mysqli_query($db, $sql2);
				while ($row = $result->fetch_row())
				{
					echo "<option value=$row[0]>$row[0]</option>";
				}
				?>
			</select><br>
		Origin Province:
			<select name="province" />
				<option hidden disabled selected value> -- select Province -- </option>
				<?php
				$sql2 = "SELECT nazev_oblast FROM Oblast;";
				$result = mysqli_query($db, $sql2);
				while ($row = $result->fetch_row())
				{
					echo "<option value=$row[0]>$row[0]</option>";
				}
				?>
			</select><br>
		Quality: <input type="text" id="quality" name="quality" value="<?php echo isset($_POST["quality"]) ? htmlspecialchars($_POST["quality"]) : "" ?>"/><br>
		
        <input type="submit" name="add" value="Confirm" />
</form>
<?php
// Remove chosen tea
if (isset($_POST["add"]))
{
	if ((isset($_POST["name"])) && (isset($_POST["distributor"])) &&
		(isset($_POST["type"])) && (isset($_POST["country"])) &&
		$_POST["name"] != "" && $_POST["distributor"] != "" &&
		strlen($_POST["name"]) <= 25 && strlen($_POST["distributor"]) <= 45 &&
		isset($_POST["quality"]) && strlen($_POST["quality"]) <= 10)
	{
		$name = $_POST["name"];
		$dist = $_POST["distributor"];
		$type = $_POST["type"];
		$country = $_POST["country"];
		$quality = $_POST["quality"];
		$brew = $_POST["brew"];
		$null = "NULL";
		if ($quality == "")
			$quality = $null;
		else
			$quality = "'$quality'";
		$province = $null;
		if (isset($_POST["province"]) && strlen($_POST["province"]) <= 25)
		{
			$province = $_POST["province"];
			if ($province == "")
				$province = $null;
			else
				$province = "'$province'";
		}
		if ($brew == "")
			$brew = $null;
		else 
			$brew = "'$brew'";

		$sql3 = "INSERT INTO Caj 
				(nazev, druh, dodavatel, doba_louhovani, kvalita, 
				zeme_puvodu, nazev_oblasti)
				VALUES('$name', '$type', '$dist', $brew, $quality, 
				'$country', $province);";
		mysqli_query($db, $sql3);
		echo mysqli_error($db);
	}
	else
	{
		if (strlen($_POST["quality"]) > 10)
			echo "<p>Quality cannot be longer that 10 characters</p>";
		else if (strlen($_POST["distributor"]) > 45)
			echo "<p>Distributor cannot be longer that 45 characters</p>";
		else if (strlen($_POST["name"]) > 25)
			echo "<p>Tea name cannot be longer that 25 characters</p>";
		else	
			echo "<p>Please fill all required windows (marked *)</p>";
	}
}
?>
	
</div>


<div id="PortalPage">
<h3>Remove Tea</h3>
<p>Please choose tea to be removed</p>
<!-- Tea removing form -->
<form action="<?=$_SERVER["PHP_SELF"]?>"  method="post">
<select name="Teas">
<option hidden disabled selected value> -- select Tea -- </option>
<?php
$sql1 = "SELECT id, nazev, druh, dodavatel, 
		zeme_puvodu, doba_louhovani, kvalita, nazev_oblasti
		FROM Caj;";
$result = mysqli_query($db, $sql1);
while ($row = $result->fetch_row())
{
	echo "<option value=$row[0]>$row[0] : $row[1]</option>";
}
?>

</select>
<input type="submit" name="remove" value="Confirm" />
</form>

<?php
// Remove chosen tea
if (isset($_POST["remove"]))
{
	$selected_tea = $_POST["Teas"];
	$sql = "DELETE FROM Caj WHERE id=$selected_tea";
	if (mysqli_query($db, $sql))
	{
		header("Location: ./tea_options.php");
		echo "Batch removed";
	}
	else
	{
		echo "This Tea cannot be deleted, it is used in existing batch";
	}
}
?>
</div>


<div id="PortalPageRightWrapper">
<div id="PortalPageRight">
<h3>Tea Database</h3>

<?php
echo "<table>";
	echo "<tr>";
		echo "<th> Tea ID </th>";
		echo "<th> Name </th>";
		echo "<th> Distributor </th>";
		echo "<th> Country </th>";
		echo "<th> Province </th>";
		echo "<th> Brew Time </th>";
		echo "<th> Quality </th>";
	echo "</tr>";

$result = mysqli_query($db, $sql1);
while ($row = $result->fetch_row()) 
{
	printf("<tr>\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n</tr>\n",
			$row[0], $row[1], $row[3], $row[4], $row[7], $row[5], $row[6]);
}
echo "</table>";
?>


</div>

</div>
</body>



</html>




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
<h1>Administrator options</h1>
<?php
	//session_start();
	if (isset($_SESSION["username"]) && isset($_SESSION["role"]) 
		&& $_SESSION["role"] == "admin")
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
<h3>Change administrative rights</h3>
<p>Please choose employee to change his administrative rights</p>
<form action="<?=$_SERVER["PHP_SELF"]?>"  method="post">
<select name="Employees">
<option hidden disabled selected value> -- select Employee -- </option>
<?php
$sql1 = "SELECT Osoba.id, jmeno , login, pozice, 
		adresa, plat , pohlavi, kontakt
		FROM Osoba NATURAL JOIN Zamestnanec;";
$result = mysqli_query($db, $sql1);
while ($row = $result->fetch_row())
{
	echo "<option value=$row[0]>$row[0] : $row[1]</option>";
}
?>
</select>
New role: <select name="role" />
			<option hidden disabled selected value> -- New role -- </option>
			<option value = "admin">Administrator</option>
			<option value = "emp">Employee</option>
          </select><br> 
<input type="submit" name="remove" value="Confirm" />
</form>

<?php
if (isset($_POST["remove"]))
{
	if (isset($_POST["Employees"]) && isset($_POST["role"]))
	{
		$selected_employee = $_POST["Employees"];
		$role = $_POST["role"];
		
		$is_admin = 0;
		$sql1 = "SELECT id_zamestnanec FROM Admin;";
		$result = mysqli_query($db, $sql1);
		while ($row = $result->fetch_row()) 
			if ($row[0] == $selected_employee)
			{
				$is_admin = 1;
				break;
			}
		if ($role == "admin" && $is_admin == 1)
			echo "<p>User is already administrator</p>";
		else if ($role == "emp" && $is_admin == 0)
			echo "<p>User is already employee</p>";
		else if ($role == "admin")
		{
			$sql1 = "INSERT INTO Admin (id_zamestnanec) VALUES ($selected_employee);";
			mysqli_query($db, $sql1);
			echo "<p>Administrator updated</p>";
			header("Location: ./admin_options.php");
		}
		else if ($role == "emp")
		{
			$sql1 = "DELETE FROM Admin WHERE id_zamestnanec = $selected_employee;";
			mysqli_query($db, $sql1);
			echo "<p>Employee updated</p>";
			header("Location: ./admin_options.php");
		}
		
		
	}
	else
		echo "<p>Please choose employee and new function</p>";
}
?>
</div>


<div id="PortalPageRightWrapper">
<div id="PortalPageRight">
<h3>Administrators</h3>
<table>
	<tr>
		<th> ID </th>
		<th> Login </th>
	</tr>
<?php

$sql1 = "SELECT Osoba.id, login FROM Admin JOIN Osoba ON Osoba.id = id_zamestnanec GROUP BY Osoba.id;";
$result = mysqli_query($db, $sql1);
if ($result->num_rows)
	while ($row = $result->fetch_row()) 
	{
		printf("<tr>\n<th>%s</th>\n<th>%s</th>\n</tr>\n",
				$row[0], $row[1]);
	}
?>
</table>

<h3>Employees</h3>
<table>
	<tr>
		<th> ID </th>
		<th> Login </th>
	</tr>
<?php
$sql1 = "SELECT Osoba.id, login FROM Osoba NATURAL JOIN Zamestnanec
		WHERE Osoba.id NOT IN (SELECT id_zamestnanec FROM Admin) GROUP BY Osoba.id;";
$result = mysqli_query($db, $sql1);
if ($result->num_rows)
{
	while ($row = $result->fetch_row()) 
	{
		printf("<tr>\n<th>%s</th>\n<th>%s</th>\n</tr>\n",
				$row[0], $row[1]);
	}
}
?>
</table>
</div>
</div>
</body>



</html>




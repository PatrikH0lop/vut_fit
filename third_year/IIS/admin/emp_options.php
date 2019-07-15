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
<h1>Employee options</h1>
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
<h3>Hire new employee</h3>
<p>Please fill the form to hire new employee (* = required field)</p>
<!-- Employee form -->
<form action="<?=$_SERVER["PHP_SELF"]?>"  method="post">
        Full name*: <input type="text" id="name" name="name" value="<?php echo isset($_POST["name"]) ? htmlspecialchars($_POST["name"]) : "" ?>" /> <br>
        Position*:&nbsp&nbsp&nbsp
		<input type="text" id="distributor" name="distributor" value="<?php echo isset($_POST["distributor"]) ? htmlspecialchars($_POST["distributor"]) : "" ?>" /><br>
        Gender*:  
			<select name="type" />
				<option value = "male">Male</option>	
				<option value = "female">Female</option>	
			</select>&nbsp&nbsp&nbsp&nbsp&nbsp
        Monthly wage*: <input type = "number" id="wage" name="wage" value="<?php echo isset($_POST["wage"]) ? $_POST["wage"] : 0 ?>"/><br>
        Account*: &nbsp
		<input type = "text" id="account" name="account" value="<?php echo isset($_POST["account"]) ? htmlspecialchars($_POST["account"]) : "" ?>" /><br>
		Address*: &nbsp
		<input type="text" id="address" name="address" value="<?php echo isset($_POST["address"]) ? htmlspecialchars($_POST["address"]) : "" ?>"/><br>
		Contact*: &nbsp&nbsp
		<input type="text" id="contact" name="contact" value="<?php echo isset($_POST["contact"]) ? htmlspecialchars($_POST["contact"]) : "" ?>"/><br>
		Login*: &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
		<input type="text" id="login" name="login" value="<?php echo isset($_POST["login"]) ? htmlspecialchars($_POST["login"]) : "" ?>"/><br>
		Password*:&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp 
		<input type="password" id="password1" name="password1"/><br>
		Repeat password*: <input type="password" id="password2" name="password2"/><br>
		Date of Birth*:&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
		<input type="date" id="date" name="date"/><br>
        <input type="submit" name="add" value="Confirm" />
</form>
<?php
if (isset($_POST["add"]))
{
	if (isset($_POST["name"]) && isset($_POST["distributor"]) &&
		isset($_POST["type"]) && isset($_POST["wage"]) && 
		isset($_POST["address"]) && isset($_POST["contact"]) &&
		isset($_POST["login"]) && isset($_POST["password1"]) &&
		isset($_POST["password2"]) && isset($_POST["date"]) &&
		isset($_POST["account"])) 
	{
		$name = $_POST["name"];
		$position = $_POST["distributor"];
		$gender = $_POST["type"];
		$wage = $_POST["wage"];
		$address = $_POST["address"];
		$contact = $_POST["contact"];
		$login = $_POST["login"];
		$account = $_POST["account"];
		$pass1 = $_POST["password1"];
		$pass2 = $_POST["password2"];
		$date = $_POST["date"];
		$null = "NULL";
		if ($name == "" || $position == "" || $wage == 0 || $address == "" ||
			$contact == "" || $login == "" || $account == "" || $pass1 == "" ||
			$date == "")
			echo "<p>Please fill all required windows (marked *)</p>";
		else if ($pass1 != $pass2)
			echo "<p>Passwords do not match</p>";
		else if(strlen($name) > 25)
			echo "<p>Name is too long</p>";
		else if(strlen($address) > 25)
			echo "<p>Address is too long</p>";
		else if(strlen($contact) > 25)
			echo "<p>Contact info is too long</p>";
		else if(strlen($login) > 25)
			echo "<p>Login is too long</p>";
		else if(strlen($pass1) > 25)
			echo "<p>Password is too long</p>";
		else if(strlen($position) > 25)
			echo "<p>Position name is too long</p>";
		else
		{
			$sql3 = "SELECT id, login From Osoba;"; 
			$result = mysqli_query($db, $sql3);
			$last_id = 0;
			$logins = array();
			while ($row = $result->fetch_row())
			{
				$last_id = $row[0];
				array_push($logins, $row[1]);
			}
			$last_id += 1;
			
			if (in_array($login, $logins))
				echo "<p>This login is already used</p>";
			else
			{
				$sql3 = "INSERT INTO Osoba 
					(id, jmeno, adresa, datum_narozeni, kontakt, login, heslo)
					VALUES($last_id, '$name', '$address', '$date', '$contact', 
					'$login', '$pass1');";
				mysqli_query($db, $sql3);
				echo mysqli_error($db);
				$sql3 = "INSERT INTO Zamestnanec 
						(id, pozice, pohlavi, cislo_uctu, plat)
						VALUES($last_id, '$position', '$gender', '$account', $wage);";
				mysqli_query($db, $sql3);
				echo mysqli_error($db);
			}
		}
	}
	else
	{
		echo "<p>Please fill all required windows (marked *)</p>";
	}
}
?>
	
</div>


<div id="PortalPage">
<h3>Fire employee</h3>
<p>Please choose employee to be fired</p>
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
<input type="submit" name="remove" value="Confirm" />
</form>

<?php
if (isset($_POST["remove"]))
{
	if (isset($_POST["Empoloyees"]))
	{
		$selected_employee = $_POST["Employees"];
		$sql = "DELETE FROM Zamestnanec WHERE id=$selected_employee";
		if (mysqli_query($db, $sql))
		{
			$sql = "DELETE FROM Osoba WHERE id=$selected_employee";
			mysqli_query($db, $sql);
			header("Location: ./emp_options.php");
		}
		else
		{
			echo "This Employee cannot be terminated, he/she is currently working on order";
		}
	}
	else
	echo "<p>Please choose employee</p>";
}
?>
</div>


<div id="PortalPageRightWrapper">
<div id="PortalPageRight">
<h3>Employee Database</h3>

<?php
echo "<table>";
	echo "<tr>";
		echo "<th> ID </th>";
		echo "<th> Name </th>";
		echo "<th> Login </th>";
		echo "<th> Position </th>";
		echo "<th> Address </th>";
		echo "<th> Contact </th>";
		echo "<th> Wage </th>";
		echo "<th> Gender </th>";
	echo "</tr>";

$result = mysqli_query($db, $sql1);
while ($row = $result->fetch_row()) 
{
	printf("<tr>\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n</tr>\n",
			$row[0], $row[1], $row[2], $row[3], $row[4], $row[7], $row[5], $row[6]);
}
echo "</table>";
?>


</div>

</div>
</body>



</html>




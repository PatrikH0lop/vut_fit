<html>
<header><title>IIS Project 2018</title></header>

<head>
<link rel="stylesheet" href="./css/style.css">
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
<h1>Registration</h1>
<p><a href="./index.php">Back to login</a></p>
</div>

<div id="PortalPage">
<h3>Register new user</h3>
<p>Please fill the form (* = required field)</p>

<form action="<?=$_SERVER["PHP_SELF"]?>"  method="post">
        Full name*: <input type="text" id="name" name="name" value="<?php echo isset($_POST["name"]) ? htmlspecialchars($_POST["name"]) : "" ?>" /> <br>
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
	if (isset($_POST["name"]) && isset($_POST["address"]) &&
		isset($_POST["contact"]) && isset($_POST["login"]) && 
		isset($_POST["password1"]) && isset($_POST["password2"]) && 
		isset($_POST["date"])) 
	{
		$name = $_POST["name"];
		$address = $_POST["address"];
		$contact = $_POST["contact"];
		$login = $_POST["login"];
		$pass1 = $_POST["password1"];
		$pass2 = $_POST["password2"];
		$date = $_POST["date"];
		$null = "NULL";
		if ($name == "" || $address == "" || $contact == "" || $login == "" ||
			$pass1 == "" || $date == "")
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
				$reg = date("Y-m-d");
				$sql3 = "INSERT INTO Odberatel
						(id, vernostni_body, datum_registrace)
						VALUES($last_id, 0, '$reg');";
				mysqli_query($db, $sql3);
				header("Location: ./index.php");
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
</body>
</html>




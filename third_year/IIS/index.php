<html>
<header><title>IIS Project 2018</title></header>

<head>
<link rel="stylesheet" href="./css/style.css">
</head>

<body>

<div id="MainPage">
<h1>IIS Project 2018 - Tea Distribution</h1>
<p>Authors:</p>
<p>Matej Hrabal - xhraba12</p>
<p>Patrik Holop - xholop01</p>
<p><a href="./doc.html">Documentation</a>
&nbsp&nbsp&nbsp&nbsp
<a href="./registration.php">Register new user</a></p>
</div>

<div id="Login">
<!-- Login form -->
<form action="<?=$_SERVER["PHP_SELF"]?>"  method="post">
        Username: <input type="text" id="username" name="username" value="<?php echo isset($_POST["username"]) ? htmlspecialchars($_POST["username"]) : "" ?>" /> <br>
        Password:&nbsp <input type="password" id="password" name="password" />
        <br><!-- Button spacing -->
		&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
		&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp
        <input type="submit" name="login" value="Login" />
</form>

<?php
/*
	Database initialization
*/
$db = mysqli_init();

if (!(mysqli_real_connect($db, 'localhost', 'REDACTED', 'REDACTED', 'REDACTED', 0, '/var/run/mysql/mysql.sock')))
{
	die('cannot connect '.mysqli_connecterror());
}

// Start the session
session_start();
unset($_SESSION["username"]);
unset($_SESSION["role"]);
unset($_SESSION["password"]);
unset($_SESSION["obj_id"]);
unset($_SESSION["new_obj"]);

?>

<?php
// Function for quick password confirmation and portal forwarding
function check_and_forward($result, $is_customer)
{
	if ($row = $result->fetch_row()) 
	{
		if ($row[1] == htmlspecialchars($_POST["password"])) 
		{
			$_SESSION["username"] = $_POST["username"];
			if ($is_customer)
			{
				$_SESSION["time"] = time();
				$_SESSION["role"] = "customer";
				header("Location: ./customer/index.php");
			}
			else
			{
				$db = mysqli_init();

				if (!(mysqli_real_connect($db, 'localhost', 'REDACTED', 'REDACTED', 'REDACTED', 0, '/var/run/mysql/mysql.sock')))
				{
					die('cannot connect '.mysqli_connecterror());
				}

				$sql_admin = "SELECT id FROM Admin WHERE id_zamestnanec=$row[0]";
				$admin_result = mysqli_query($db, $sql_admin);
				
				if ($admin_result !== False && $admin_result->num_rows !== 0) {
					$_SESSION["time"] = time();
					$_SESSION["role"] = "admin";
					header("Location: ./admin/index.php");
				} else {
					$_SESSION["time"] = time();
					$_SESSION["role"] = "employee";
					header("Location: ./employee/index.php");
				}
			}
		}
		else 
		{
			echo "Wrong password";
		}
	} 
}

//Form submitted
if(isset($_POST["login"]))
{
	//Error checking
	if(!$_POST["username"] or !$_POST["password"]) 
	{
			echo "<p>&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp";
			echo "&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp";
			echo "Please provide valid username and password</p>\n";
	}
	if($_POST['username'] and $_POST['password'])
	{
		/*
			Comparing username and password against our database
			If username and password matches customer, they are 
			forwarded to customer portal, and if it matches employee
			credentials, then employee is forwarded to employee portal  
		*/
		$sql_customer = "SELECT Odberatel.id, heslo FROM Odberatel NATURAL JOIN Osoba WHERE login = '{$_POST['username']}';";
		$result = mysqli_query($db, $sql_customer);
		if ($result !== False && $result->num_rows !== 0) 
		{
			check_and_forward($result, True);
		}
		else 
		{
			$sql_customer = "SELECT Zamestnanec.id, heslo FROM Zamestnanec NATURAL JOIN Osoba WHERE login = '{$_POST['username']}';";
			$result = mysqli_query($db, $sql_customer);
	
			if ($result !== False && $result->num_rows !== 0) 
			{
				check_and_forward($result, False);
			}
			else 
			{
				echo "Username not found";
			}
		}
	} 
}
?>
</div>

<div id="Login">
<p>For testing purposes please use following credentials</p>
<table>
	<tr>
    	<th> Username </th>
    	<th> Password </th>
    	<th> Role </th>
	</tr>
	<tr>
    	<th> admin </th>
    	<th> admin </th>
    	<th> For testing administrator system, can switch to employee </th>
	</tr>
	<tr>
    	<th> employee </th>
    	<th> employee </th>
    	<th> For testing employee system </th>
	</tr>
	<tr>
    	<th> user </th>
    	<th> user </th>
    	<th> For testing customer system </th>
	</tr>
</table>
</div>
</body>
</html>


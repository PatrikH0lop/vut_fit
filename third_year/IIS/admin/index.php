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

<body>

<div id="PortalPageEmp">
<h1>Administrator portal</h1>
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
<a href="../employee/index.php">Switch to employee portal</a></p>
</div>

<div id="PortalPageEmp">
<h3>Administrator options</h3>
<form name="emp_opt" action="<?=$_SERVER["PHP_SELF"]?>" method="post">
	<input type="submit" name="add_emp" value="Hire/Fire employee"/>
	<input type="submit" name="add_admin" value="Change admin rights"/>
</form>

<?php
	if(isset($_POST["add_emp"]))
	{
		header("Location: ./emp_options.php");
	}
	if(isset($_POST["add_admin"]))
	{
		header("Location: ./admin_options.php");
	}
?>

</div>
</body>

</html>




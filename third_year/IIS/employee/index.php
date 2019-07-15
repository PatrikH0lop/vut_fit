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
<h1>Employee portal</h1>
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
<?php
	if ($_SESSION["role"] == "admin")
		echo "<a href='../admin/index.php'>Administrator portal</a>";
?>
</p>
</div>

<div id="PortalPageEmp">
<h3>Employee options</h3>
<form name="emp_opt" action="<?=$_SERVER["PHP_SELF"]?>" method="post">
	<input type="submit" name="add_batch" value="Add or remove batch"/>
	<input type="submit" name="add_tea" value="Add or remove tea"/>
	<input type="submit" name="change_ord" value="Change order"/>
	<input type="submit" name="add_country" value="Add or remove country"/>
	<input type="submit" name="add_province" value="Add or remove province"/>
	<input type="submit" name="discounts" value="Set discounts"/>
</form>

<?php
	if(isset($_POST["add_tea"]))
	{
		header("Location: ./tea_options.php");
	}
	if(isset($_POST["add_batch"]))
	{
		header("Location: ./batch_options.php");
	}
	if(isset($_POST["change_ord"]))
	{
		header("Location: ./order_options.php");
	}
	if(isset($_POST["add_country"]))
	{
		header("Location: ./country_options.php");
	}
	if(isset($_POST["add_province"]))
	{
		header("Location: ./province_options.php");
	}
	if(isset($_POST["discounts"]))
	{
		header("Location: ./discounts.php");
	}
?>

</div>
</body>

</html>




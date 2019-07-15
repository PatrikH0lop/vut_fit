<html>

<header>

	<link href='http://fonts.googleapis.com/css?family=Merienda+One' rel='stylesheet' type='text/css'>

    <title>
        IIS Project 2018
    </title>
</header>

<style>

input[type=text], input[type=password] {
	margin 10px 0;
	width: 100%;
	padding: 3px 5px;
	padding-bottom: 5px;
}

input[type=submit] {
	padding: 5px 15px;
	margin-top: 20px;
	width: 100%;
	font-family: Arial;
}

.login_label {
	font-family: Arial;
	font-weight: bold;
	padding-top: 5px;
}

.login_form {
	width: 30%;
	margin: auto auto;
	vertical-align: middle;
	position: absolute;
	top: 50%;
	left: 50%;
	transform:translate(-50%, -50%);
}

.main_label {
	position: absolute;
	top:25%;
	left:50%;
	transform:translate(-50%, -50%);
}

.bottom_label {
	position: absolute;
	top: 88%;
	left: 80%;
	transform:translate(-50%, -50%);
	text-align: center;
}

h4 {
	text-align: center;
}

p {
	text-align: center;
	font-weight: bold;
	font:  400 100px/1.3 'Merienda One', Helvetica, sans-serif;
	font-size: 30px;
}

body {
	background-image: url("./images/image_bg.jpg");
	background-size: cover;
	width: 100%;
	height: 100%;
}

</style>

<head>
</head>

<body>

	<div class="main_label">
	    <p> AIM </p>
		<p> Your Asset Inventory Management Software </p>
	</div>

	<div class="login_form">

		<form action="<?$_SERVER["PHP_SELF"]?>" method="post">

			<div class="login_label"> Username </div>
			<input type="text" name="username" placeholder="Username"> </input> <br>

			<div class="login_label"> Password </div>
			<input type="password" name="password" placeholder="Password"> </input> <br>
		
			<input type="submit" name="submit" value="Login"></input>

		</form>
<?php

$db = mysqli_init();
if (!mysqli_real_connect($db, 'localhost', '[REDACTED]', '[REDACTED]', '[REDACTED]', 0, '/var/run/mysql/mysql.sock')) {
	echo mysqli_connecterror();
}
session_start();

if (isset($_POST["username"]) and isset($_POST["password"])) {
	// Check username and password
	$find_user = "SELECT id FROM employee WHERE login='{$_POST["username"]}' and password='{$_POST["password"]}'";
	$result = mysqli_query($db, $find_user);
	if ($result !== False and $result->num_rows !== 0) {
		$row = $result->fetch_row();
		$user_id = $row[0];
		$_SESSION["employee"] = $user_id;
		$find_admin = "SELECT id FROM administrator WHERE id_employee='$user_id'";
		$is_admin = mysqli_query($db, $find_admin);
		$row = $is_admin->fetch_row();
		if ($row[0] == $user_id) {
			$_SESSION["role"] = "admin";
			header("Location: ./admin/admin.php");
		} else {
			$_SESSION["role"] = "user";
			header("Location: ./user/user.php");
		}
	} else {
		echo "<h4>Invalid credentials</h4>";
	}
}

?>

	</div>

	<div class="bottom_label">
		ITU Projekt 2018 <br>
		<i> Design by Patrik Holop, Matej Hrabal, Tomas Chocholaty </i>
	</div>


</body>

</html>

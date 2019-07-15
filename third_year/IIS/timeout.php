<?php

function check_timeout() {
	$current_time = time();
	if ($current_time - $_SESSION["time"] > 600) {
		unset($_SESSION["username"]);
		unset($_SESSION["time"]);
		header("Location: ../index.php");
		exit;
	}
	$_SESSION["time"] = time();
}

?>

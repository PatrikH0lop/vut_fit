<html>
<style>
body {
        background-image: url("../images/user_page.jpg");
        background-size: cover;
}
</style>


<head>
<link rel="stylesheet" href="../css/style.css">
</head>


<body>

<div id="Options">
<a href="../index.php">Logout</a>
&nbsp&nbsp&nbsp
<a href="./user.php">Back to portal</a>
&nbsp&nbsp&nbsp
</div>

<?php
session_start();
$db = mysqli_init();
if (!(mysqli_real_connect($db, 'localhost', '[REDACTED]', '[REDACTED]', '[REDACTED]', 0, '/var/run/mysql/mysql.sock'))) 
{
	die('cannot connect '.mysqli_connecterror());
}
?>

<div class="horizontal_layout">
<div id="Main">
<p>Room map</p>
<form action="<?=$_SERVER["PHP_SELF"]?>" method="post" name="assign_form">
<select id="room" name="room">
<option hidden disabled selected value> -- select Room -- </option>
<option value=1> Canteen </option>
<option value=2> Conference </option>
<option value=3> Meeting B02 </option>
<option value=4> Meeting B03 </option>
<option value=5> Office 1A </option>
</select>
&nbsp&nbsp&nbsp
<input type="submit" name="submit" value="Confirm" width="30%"> </input>
</form><br>

<?php
if(isset($_POST["submit"]))
{
	if (!isset($_POST["room"]))
		echo "<p>Please choose a room</p>";
	else if ($_POST["room"] == 1)
		echo "<img src='../images/rooms/1.png' alt='' border=2 height=67.8% width=100%></img>";
	else if ($_POST["room"] == 2)
		echo "<img src='../images/rooms/2.png' alt='' border=2 height=67.8% width=100%></img>";
	else if ($_POST["room"] == 3)
		echo "<img src='../images/rooms/2.png' alt='' border=2 height=67.8% width=100%></img>";
	else if ($_POST["room"] == 4)
		echo "<img src='../images/rooms/2.png' alt='' border=2 height=67.8% width=100%></img>";
	else if ($_POST["room"] == 5)
		echo "<img src='../images/rooms/2.png' alt='' border=2 height=67.8% width=100%></img>";

}
?>
</div>

<div id="PersonalInfo">
<p> Room equipment </p>
<table>
<tr>
        <th>Item ID</th>
        <th>type</th>
        <th>name</th>
        <th>image</th>
</tr>

<?php
if(isset($_POST["submit"]))
{
	if (isset($_POST["room"]))
	{
	$id = $_POST["room"];
	$sql = "SELECT id, type, name FROM asset WHERE id_room = $id;";
	$result = mysqli_query($db, $sql);
	while ($row = $result->fetch_row())
	{
	        printf("<tr>\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n",
	                        $row[0], $row[1], $row[2]);
	
	
	        echo "<td width='100'>\n";
		       	if ($row[1] == "mouse")
	                echo "<img src='../images/assets/mouse.png' alt='' border=2 height=100 width=100></img>";
    	    else if ($row[1] == "keyboard")
    	            echo "<img src='../images/assets/keyboard.png' alt='' border=2 height=100 width=100></img>";
    	    else if ($row[1] == "notebook")
    	            echo "<img src='../images/assets/notebook.png' alt='' border=2 height=100 width=100></img>";
    	    else if ($row[1] == "monitor")
    	            echo "<img src='../images/assets/monitor.png' alt='' border=2 height=100 width=100></img>";
    	    else
    	            echo "<img src='../images/assets/missing.png' alt='' border=2 height=100 width=100></img>";
    	    echo "</td>\n";
    	    echo "</tr>\n";
	}
	}
}

?>
</table>

</div>
</div>


</body>
</html>

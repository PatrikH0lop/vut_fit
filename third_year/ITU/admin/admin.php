<html>
<?php
$db = mysqli_init();
if (!(mysqli_real_connect($db, 'localhost', '[REDACTED]', '[REDACTED]', '[REDACTED]', 0, '/var/run/mysql/mysql.sock')))
{
    die('cannot connect '.mysqli_connecterror());
}
?>

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
    <a href="admin.php">Home</a>
    &nbsp&nbsp&nbsp
    <a href="../user/user.php">Switch to user</a>
    &nbsp&nbsp&nbsp
    <a href="placeholder.php">Asset Database</a>
</div>

<style>
@media only screen and (max-width: 1000px) {
	#MainReport {
		width: 100%;
	}
	#PersonalInfo {
		width: 100%;
	}
	#Actions {
		width: 100%;
		float: none;
	}
}
</style>

<div class="horizontal_layout">

    <div id="MainReport" class="RInfo">
        <p>Reports:</p>
        <table>
            <tr>
                <th>Date</th>
                <th>Employee</th>
                <th>Report</th>
            </tr>
            <?php

            $sql = "SELECT date, first_name, last_name, report_text  FROM employeeReport LEFT JOIN employee ON employeeReport.id_employee=employee.id;";
            $result = mysqli_query($db, $sql);
            echo "<tr>";
            if ($result != false) {
                while ($row = $result->fetch_row()) {
                    printf("<td id='reportColumn1'>%s</td>\n<td id='reportColumn2'>%s</td>\n<td id='reportColumn3'>%s</td>\n",
                        $row[0], $row[1] . " " . $row[2], $row[3]);
                    echo "</tr>\n";
                }
            }
            ?>

        </table>
    </div>

    <div id="PersonalInfo" class="PInfo">
        <p> Personal information </p>
        <img class="photo" src="../images/admin_photo.jpeg" height="150" width="300">
        <table>
            <?php
            session_start();
            $id = $_SESSION["employee"];
            $pos = $_SESSION["role"];
            if ($pos == "admin")
                $pos = "Administrator";
            else
                $pos = "Employee";
            $sql = "SELECT first_name, last_name, room.name FROM employee LEFT JOIN room on room.id = employee.id_room WHERE $id = employee.id;";
            $f_name = "";
            $l_name = "";
            $room_name = "";
            $result = mysqli_query($db, $sql);
            while ($row = $result->fetch_row())
            {
                $f_name = $row[0];
                $l_name = $row[1];
                $room_name = $row[2];
            }

            echo "<tr> <th> Name </th> <td class='table_cell'> $f_name $l_name </td> </tr>";
            echo "<tr> <th> Position </th> <td class='table_cell'> $pos </td> </tr>";
            echo "<tr onclick='show_room_map_modal();' onmouseover='' style='cursor: pointer;'> <th>Working place</th> <td class='table_cell'> $room_name  </td> </tr>";
            ?>

        </table>

    </div>
</div>
<br>
<div id="Actions" class="horizontal_layout">
    <div class="action_icon">
        <input onclick="show_add_user_modal();" type="image" src="../images/icons/add_user.png" height="80" width="80"> <br>
        <span class="icon_label"> Add User </span>
    </div>
    <div class="action_icon">
        <input onclick="show_add_tool_modal();" type="image" src="../images/icons/add_equip.png" height="80" width="80"> <br>
        <span class="icon_label"> Add Asset </span>
    </div>
    <div class="action_icon">
        <input onclick="show_equip_modal();" type="image" src="../images/icons/assign.png" height="80" width="80"> <br>
        <span class="icon_label"> Assign Equipment </span> <br> <span class="icon_label"> (User) </span>
    </div>
    <div class="action_icon">
        <input onclick="show_room_modal();" type="image" src="../images/icons/assign_room.png" height="80" width="80"> <br>
        <span class="icon_label"> Assign Equipment </span> <br> <span class="icon_label"> (Room) </span>
    </div>
    <div class="action_icon">
        <input onclick="show_map_modal();" type="image" src="../images/icons/map.png" height="80" width="80"> <br>
        <span class="icon_label"> Show map </span>
    </div>
</div>

<div id="add_user" class="modal">
    <div id="add_user_form" class="modal_inside">
        <div class="fwrapper">
            <span class="close" onmouseover='' style='cursor: pointer;'><img src="../images/icons/close.png"></span> <br>
            <div class="form_olab"> Adding User </div> <br>
            <form action="<?=$_SERVER["PHP_SELF"]?>" method="post" onsubmit="return validate_add_user()" name="add_user_form">
                <div class="image_and_desc">
                    <img id="add_photo" class="photo" height=200 width=300 src="../images/no_photo.jpeg" border="1"> <br>
                    <select id = "add_user_photo_select" onchange="change_user_photo();" name="add_user_photo">
                        <option disabled selected value="no_person"> Choose Employee Photo </option>
                        <option value="person1"> Employee 1 </option>
                        <option value="person2"> Employee 2 </option>
                        <option value="person3"> Employee 3 </option>
                    </select> <br>
                </div>
                <span class="form_hint"> Name </span><input class="input_highlight" type="text" id="add_user_name" value="" name="add_user_name"> </input> <br> 
                <span class="form_hint"> Last Name </span><input class="input_highlight" type="text" id="add_user_lastname" value="" name="add_user_lastname"> </input> <br>
                <span class="form_hint"> Position: </span>
                <select id ="add_user_position" class="padded_select" name="add_user_position">
                    <option value="Employee"> Employee </option>
                    <option value="Administrator"> Administrator </option>
                </select> <br> 

                <span class="form_hint"> Workspace: </span>
                <select id="room_selection" onchange="change_room_map();" class="padded_select" name="add_user_room">
                    <option value=1 selected> Canteen </option>
                    <option value=2> Conference </option>
                    <option value=3> Meeting B02 </option>
                    <option value=4> Meeting B03 </option>
                    <option value=5> Office 1A </option>
                </select> <br> <br>

                <span class="form_hint"> Username </span> <input type="text" id="add_user_username" value="" name="add_user_username"> </input> <br>
                <span class="form_hint"> Password </span> <input type="password" id="add_user_password" value="" name="add_user_password"> </input>
                <input type="submit" name="create_user" value="Create User"> </input>
            </form>
        </div>
    </div>
</div>

<?php
if (isset($_POST["add_user_photo"]) && isset($_POST["add_user_name"]) && isset($_POST["add_user_lastname"]) && isset($_POST["add_user_username"]) && isset($_POST["add_user_password"]) && isset($_POST["add_user_position"]) && isset($_POST["add_user_room"])) {
    // all set, add user to DB
    $sql_command = "INSERT INTO employee (first_name, last_name, login, password, id_room) VALUES ('{$_POST["add_user_name"]}', '{$_POST["add_user_lastname"]}', '{$_POST["add_user_username"]}', '{$_POST["add_user_password"]}', {$_POST["add_user_room"]});";
    //echo $sql_command;
    if (!mysqli_query($db, $sql_command)) {
        //echo "Insert employee error";
        //echo mysqli_error($db);
    }
    $user_id = mysqli_insert_id($db);
    if ($_POST["add_user_position"] == "Administrator") {
        $sql_command = "INSERT INTO administrator (id_employee) VALUES ($user_id);";
        if (!mysqli_query($db, $sql_command)) {
            //echo "Insert admin error";
        };
    }
} else {
    //echo "Not everything set";
}
?>

<div id="change_equip" class="modal">
    <div id="change_equip_form" class="modal_inside">
        <div class="fwrapper">
            <span class="close" onmouseover='' style='cursor: pointer;'><img src="../images/icons/close.png"></span>
            <div class="form_olab"> Assign Equipment </div> <br>
            <form action="<?=$_SERVER["PHP_SELF"]?>" method="post" name="assign_form">
                <span class="form_hint"> Find employee: </span>
                <select id = "assign_employee" class="padded_select" onchange="choose_user_equip();" name="assign_user">
                    <?php
                    $sql_command = "SELECT id, first_name, last_name FROM employee;";
                    $result = mysqli_query($db, $sql_command);
                    if ($result !== false) {
                        while ($row = $result->fetch_row()) {
                            echo "<option value={$row[0]}> {$row[1]} {$row[2]} </option>";
                        }
                    }
                    ?>
                </select> <br> <br>

                <span class="form_hint"> Add equipment </span> <br> <br>
                <select id = "assign_equip" class="padded_select" multiple name="assign_add[]">
                    <?php
                    $sql_command = "SELECT id, type, name FROM asset WHERE id_employee IS NULL and id_room IS NULL;";
                    $result = mysqli_query($db, $sql_command);
                    if ($result !== false) {
                        while ($row = $result->fetch_row()) {
                            echo "<option value={$row[0]}>  {$row[2]} ({$row[1]}) </option>";
                        }
                    } else {
                        //echo mysqli_error($db);
                    }
                    ?>
                </select> <br> <br>

                <span class="form_hint"> Return asset </span> <br> <br>
                <select id = "remove_equip" class="padded_select" multiple name="assign_remove[]">
                    <?php
                    //echo "<option disabled hidden>No devices</option>";
                    $sql_command = "SELECT id, type, name, id_employee FROM asset WHERE id_employee IS NOT NULL;";
                    $result = mysqli_query($db, $sql_command);
                    if ($result !== false) {
                        while ($row = $result->fetch_row()) {
                            echo "<option value={$row[0]} data-empid={$row[3]}>  {$row[2]} ({$row[1]}) </option>";
                        }
                    } else {
                        //echo mysqli_error($db);
                    }
                    ?>
                </select> <br>

                <input type="checkbox" name="assign_remove_all"> Return all assets </>
            <input type="submit" name="submit" value="Submit changes">
            </form>
        </div>
    </div>
</div>

<?php
if (isset($_POST["assign_user"])) {
    if (isset($_POST["assign_add"])) {
        foreach ($_POST["assign_add"] as $add_equipment) {
            $sql_command = "UPDATE asset SET id_employee={$_POST["assign_user"]} WHERE id=$add_equipment;";
            //echo $sql_command;
            if (!mysqli_query($db, $sql_command)) {
                //echo mysqli_error($db);
            }
        }
    }
    if (isset($_POST["assign_remove"])) {
        foreach ($_POST["assign_remove"] as $remove_equipment) {
            $sql_command = "UPDATE asset SET id_employee=NULL WHERE id=$remove_equipment;";
            if (!mysqli_query($db, $sql_command)) {
                //echo mysqli_error($db);
            }
        }
    }  // && isset($_POST["assign_add"]) && isset($_POST["assign_remove_all"])) {
    if (isset($_POST["assign_remove_all"])) {
        $sql_command = "UPDATE asset SET id_employee=NULL WHERE id_employee={$_POST["assign_user"]};";
        if (!mysqli_query($db, $sql_command)) {
        }
    }
} else {
    //echo "Not all assigned";
}
?>

<div id="change_equip_room" class="modal">
    <div id="change_equip_room_form" class="modal_inside">
        <div class="fwrapper">
            <span class="close" onmouseover='' style='cursor: pointer;'><img src="../images/icons/close.png"></span>
            <div class="form_olab"> Assign Equipment </div> <br>
            <form action="<?=$_SERVER["PHP_SELF"]?>" method="post" name="assign_form">
                <span class="form_hint"> Find room: </span>
                <select id = "assign_room" class="padded_select" onchange="choose_room_equip();" name="assign_room">
                    <?php
                    $sql_command = "SELECT id, name FROM room;";
                    $result = mysqli_query($db, $sql_command);
                    if ($result !== false) {
                        while ($row = $result->fetch_row()) {
                            echo "<option value={$row[0]}> {$row[1]} </option>";
                        }
                    }
                    ?>
                </select> <br> <br>

                <span class="form_hint"> Add equipment </span> <br> <br>
                <select id = "assign_equip_room" class="padded_select" multiple name="assign_add_room[]">
                    <?php
                    $sql_command = "SELECT id, type, name FROM asset WHERE id_employee IS NULL and id_room IS NULL;";
                    $result = mysqli_query($db, $sql_command);
                    if ($result !== false) {
                        while ($row = $result->fetch_row()) {
                            echo "<option value={$row[0]}>  {$row[2]} ({$row[1]}) </option>";
                        }
                    } else {
                        //echo mysqli_error($db);
                    }
                    ?>
                </select> <br> <br>

                <span class="form_hint"> Return asset </span> <br> <br>
                <select id = "remove_equip_room" class="padded_select" multiple name="assign_remove_room[]">
                    <?php
                    //echo "<option disabled hidden>No devices</option>";
                    $sql_command = "SELECT id, type, name, id_room FROM asset WHERE id_room IS NOT NULL;";
                    $result = mysqli_query($db, $sql_command);
                    if ($result !== false) {
                        while ($row = $result->fetch_row()) {
                            echo "<option value={$row[0]} data-empid={$row[3]}>  {$row[2]} ({$row[1]}) </option>";
                        }
                    } else {
                        //echo mysqli_error($db);
                    }
                    ?>
                </select> <br>

                <input type="checkbox" name="assign_remove_all_room"> Return all assets </>
            <input type="submit" name="submit" value="Submit changes">
            </form>
        </div>
    </div>
</div>

<?php
if (isset($_POST["assign_room"])) {
    if (isset($_POST["assign_add_room"])) {
        foreach ($_POST["assign_add_room"] as $add_equipment) {
            $sql_command = "UPDATE asset SET id_room={$_POST["assign_room"]} WHERE id=$add_equipment;";
            //echo $sql_command;
            if (!mysqli_query($db, $sql_command)) {
                //echo mysqli_error($db);
            }
        }
    }
    if (isset($_POST["assign_remove_room"])) {
        foreach ($_POST["assign_remove_room"] as $remove_equipment) {
            $sql_command = "UPDATE asset SET id_room=NULL WHERE id=$remove_equipment;";
            if (!mysqli_query($db, $sql_command)) {
                //echo mysqli_error($db);
            }
        }
    }  // && isset($_POST["assign_add"]) && isset($_POST["assign_remove_all"])) {
    if (isset($_POST["assign_remove_all_room"])) {
        $sql_command = "UPDATE asset SET id_room=NULL WHERE id_room={$_POST["assign_room"]};";
        if (!mysqli_query($db, $sql_command)) {
        }
    }
} else {
    //echo "Not all assigned";
}
?>

<div id="map" class="modal">
    <div id="map_form" class="map_inside">
        <span class="close" onmouseover='' style='cursor: pointer;'><img src="../images/icons/close.png"></span>
        <div class="form_olab"> Map </div> <br>
        <div class="image_and_desc">
            <span class="form_hint"> Choose The Room </span> <br> <br>
            <select id="map_selection" onchange="change_room_map();">
                <option value=1 selected> Canteen </option>
                <option value=2> Conference </option>
                <option value=3> Meeting B02 </option>
                <option value=4> Meeting B03 </option>
                <option value=5> Office 1A </option>
            </select> <br> <br>
            <img id="map_image" class="photo" src="../images/rooms/1.png" height=500 width=750 border="2">
        </div>
    </div>
</div>

<div id="add_tool" class="modal">
    <div id="tool_form" class="modal_inside">
        <div class="fwrapper">
            <span class="close" onmouseover='' style='cursor: pointer;'><img src="../images/icons/close.png"></span>
            <div class="form_olab"> Add Asset Form </div>
            <form action="<?=$_SERVER["PHP_SELF"]?>" method="post" onsubmit="return validate_add_tool()" name="add_tool_form">
				<div class="image_and_desc">
                <img id="asset_photo" class="tool_photo" height=200 height=200 src="../images/assets/keyboard.png"> <br>
                <span class="form_hint"> Select asset type </span>
                <select id = "add_asset_type_select" onchange="change_asset_photo();" class="padded_select" name="add_tool_type">
                    <option selected value="keyboard"> Keyboard </option>
                    <option value="mouse"> Mouse </option>
                    <option value="monitor"> Monitor </option>
                    <option value="notebook"> Notebook </option>
                </select> <br> <br>
				</div>
                <span class="form_hint"> Asset Name </span> <input type="text" id="add_asset_name" value="" name="add_tool_name"> </input> <br>
                <span class="form_hint"> Asset Description </span> <input type="text" id="add_asset_description" value="" name="add_tool_desc"> </input> <br> <br>
                <span class="form_hint"> Date on evidence bill </span> <input class="padded_select" type="date" id="add_asset_date" name="add_tool_date"> </input> <br><br>
                <span class="form_hint"> Price </span> <input type="number" id="add_asset_price" name="add_tool_price"> </input>
                <input type="submit" name="create_tool" value="Create Asset"> </input>
            </form>
        </div>
    </div>
</div>

<div id="room_map" class="modal">
    <div id="map_form" class="map_inside">
        <span class="close" onmouseover='' style='cursor: pointer;'><img src="../images/icons/close.png"></span>
        <div class="form_olab"><?php echo $room_name;?></div> <br>
        <div class="image_and_desc">
            <img id="map_image" class="photo" src="../images/rooms/1.png" height=500 width=750 border="2">
        </div>
    </div>
</div>


<?php
if (isset($_POST["add_tool_name"]) && isset($_POST["add_tool_desc"]) && isset($_POST["add_tool_price"]) && isset($_POST["add_tool_date"]) && isset($_POST["add_tool_type"])) {
    $sql_command = "INSERT INTO asset (type, name, description, price, date_of_evidence) VALUES ('{$_POST["add_tool_type"]}', '{$_POST["add_tool_name"]}', '{$_POST["add_tool_desc"]}', {$_POST["add_tool_price"]}, '{$_POST["add_tool_date"]}');";
    if (!mysqli_query($db, $sql_command)) {
        //echo mysqli_error($db);
    }
} else {
    //echo "Not everything set";
}
?>

<script>

    choose_user_equip();
    choose_room_equip();

    function choose_room_equip() {

        var selection = document.getElementById("assign_room");
        var emp_id = selection.options[selection.selectedIndex].value;
        console.log(emp_id);
        var rselection = document.getElementById("remove_equip_room");
        for (var i = 0; i<rselection.options.length; i++) {
            console.log(rselection.options[i].getAttribute("data-empid"));
            if (rselection.options[i].getAttribute("data-empid") != emp_id) {
                rselection.options[i].disabled = true;
            } else {
                rselection.options[i].disabled = false;
            }
        }

        var disable = true;
        for (var i = 0; i<rselection.options.length; i++) {
            if (rselection[i].disabled !== true) disable = false;
        }
        if (disable === true) {
            rselection.style.display = 'none';
        } else {
            rselection.style.display = 'inline';
        }

    }

    function choose_user_equip() {

        var selection = document.getElementById("assign_employee");
        var emp_id = selection.options[selection.selectedIndex].value;
        console.log(emp_id);
        var rselection = document.getElementById("remove_equip");
        for (var i = 0; i<rselection.options.length; i++) {
            console.log(rselection.options[i].getAttribute("data-empid"));
            if (rselection.options[i].getAttribute("data-empid") != emp_id) {
                rselection.options[i].disabled = true;
            } else {
                rselection.options[i].disabled = false;
            }
        }

        var disable = true;
        for (var i = 0; i<rselection.options.length; i++) {
            if (rselection[i].disabled !== true) disable = false;
        }
        if (disable === true) {
            rselection.style.display = 'none';
        } else {
            rselection.style.display = 'inline';
        }

    }

    function validate_add_user() {
        var ret_val = true;
        var form = document.forms["add_user_form"];

        if (form["add_user_name"].value == "" || /\s/.test(form["add_user_name"].value)) {
            document.getElementById("add_user_name").style.borderColor = "red";
            ret_val = false;
        }
        if (form["add_user_lastname"].value == "" || /\s/.test(form["add_user_lastname"].value)) {
            document.getElementById("add_user_lastname").style.borderColor= "red";
            ret_val = false;
        }
        if (form["add_user_username"].value == "" || /\s/.test(form["add_user_username"].value)) {
            document.getElementById("add_user_username").style.borderColor = "red";
            ret_val = false;
        }
        if (form["add_user_password"].value == "" || /\s/.test(form["add_user_password"].value)) {
            document.getElementById("add_user_password").style.borderColor = "red";
            ret_val = false;
        }
        if (form["add_user_photo"].value == "no_person") {
            document.getElementById("add_user_photo_select").style.borderColor = "red";
            ret_val = false;
        }
        console.log(ret_val);
        return ret_val;
    }

    function validate_add_tool() {
        var ret_val = true;
        var form = document.forms["add_tool_form"];

        if (form["add_tool_type"].value == "missing") {
            document.getElementById("add_asset_type_select").style.borderColor = "red";
            ret_val = false;
        }
        if (form["add_tool_name"].value == "" || /\s/.test(form["add_tool_name"].value)) {
            document.getElementById("add_asset_name").style.borderColor = "red";
            ret_val = false;
        }
        if (form["add_tool_desc"].value == "" || /\s/.test(form["add_tool_desc"].value)) {
            document.getElementById("add_asset_description").style.borderColor = "red";
            ret_val = false;
        }
        console.log(form["add_tool_date"].value);
        if (form["add_tool_date"].value == "" || !(/^\d{4}-\d{2}-\d{2}/.test(form["add_tool_date"].value))) {
            document.getElementById("add_asset_date").style.borderColor = "red";
            ret_val = false;
        }
        if (form["add_tool_price"].value == "" || form["add_tool_price"].value <= 0) {
            document.getElementById("add_asset_price").style.borderColor = "red";
            ret_val = false;
        }

        return ret_val;
    }

    function path_to_images() {
        var url = window.location.href;
        var login = url.substr(30, 8);
        return "http://www.stud.fit.vutbr.cz/~" + login + "/ITU/images";
    }

    function change_room_map() {
        var selection = document.getElementById("map_selection");
        var option = selection.options[selection.selectedIndex].value;
        var img = document.getElementById("map_image");
        img.src = path_to_images() + "/rooms/" + option + '.png';
    }

    function change_user_photo() {
        var selection = document.getElementById("add_user_photo_select");
        var option = selection.options[selection.selectedIndex].value;
        var img = document.getElementById("add_photo");
        img.src = path_to_images() + "/persons/" + option;
    }

    function change_asset_photo() {
        var selection = document.getElementById("add_asset_type_select");
        var option = selection.options[selection.selectedIndex].value;
        var img = document.getElementById("asset_photo");
        img.src = path_to_images() + "/assets/" + option + '.png';
    }

    document.getElementsByClassName("close")[4].onclick = function() {
        document.getElementById("add_tool").style.display = "none";
    }

    document.getElementsByClassName("close")[0].onclick = function() {
        document.getElementById("add_user").style.display = "none";
    }

    document.getElementsByClassName("close")[1].onclick = function() {
        document.getElementById("change_equip").style.display = "none";
    }

    document.getElementsByClassName("close")[3].onclick = function() {
        document.getElementById("map").style.display = "none";
    }

    document.getElementsByClassName("close")[5].onclick = function() {
        document.getElementById("room_map").style.display = "none";
    }

    document.getElementsByClassName("close")[2].onclick = function() {
        document.getElementById("change_equip_room").style.display = "none";
    }

    function show_add_tool_modal() {
        document.getElementById("add_tool").style.display = "block";
    }

    function show_add_user_modal() {
        document.getElementById("add_user").style.display = "block";
    }

    function show_equip_modal() {
        document.getElementById("change_equip").style.display = "block";
    }

    function show_room_modal() {
        document.getElementById("change_equip_room").style.display = "block";
    }

    function show_map_modal() {
        document.getElementById("map").style.display = "block";
    }

    function show_room_map_modal() {
        document.getElementById("room_map").style.display = "block";
    }

</script>

</body>
</html>

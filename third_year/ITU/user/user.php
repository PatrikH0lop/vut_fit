<?php

include 'dbh.php';

?>
<html>
<style>
    body {
        background-image: url("../images/user_page.jpg");
        background-size: cover;
    }
</style>


<head>
    <link rel="stylesheet" href="../css/style.css">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.0/jquery.min.js"></script>
    <script type="text/javascript" src="user.js"> </script>
</head>


<body>

<div id="Options">
    <a href="../index.php">Logout</a>
    <?php
    session_start();
    if ($_SESSION["role"] == "admin") {
        echo "&nbsp&nbsp&nbsp";
        echo "<a href='../admin/admin.php'>Switch to admin</a>";
    }
    ?>
    &nbsp&nbsp&nbsp
    &nbsp&nbsp&nbsp
</div>

<div id="data"></div>

<div class="horizontal_layout">
    <div id="Main">
        <p>Your workstation:</p>
        <table>
            <tr>
                <th>Item ID</th>
                <th>type</th>
                <th>name</th>
                <th>image</th>
            </tr>

            <?php
            $id = $_SESSION["employee"];
            $sql = "SELECT id, type, name FROM asset WHERE id_employee = $id;";
            $result = mysqli_query($db, $sql);
            while ($row = $result->fetch_row()) {
                printf("<tr onmouseover=\"MouseOver(this, %s);\" onmouseout=\"MouseOut(this);\">\n<th>%s</th>\n<th>%s</th>\n<th>%s</th>\n",
                    $row[0] ,$row[0], $row[1], $row[2]);


                echo "<td width='100'>\n";
                if ($row[1] == "mouse")
                    echo "<img src='../images/assets/mouse.png' alt='' border=2 height=80 width=100></img>";
                else if ($row[1] == "keyboard")
                    echo "<img src='../images/assets/keyboard.png' alt='' border=2 height=80 width=100></img>";
                else if ($row[1] == "notebook")
                    echo "<img src='../images/assets/notebook.png' alt='' border=2 height=80 width=100></img>";
                else if ($row[1] == "monitor")
                    echo "<img src='../images/assets/monitor.png' alt='' border=2 height=80 width=100></img>";
                else if ($row[1] == "GPU")
                    echo "<img src='../images/assets/GPU.png' alt='' border=2 height=80 width=100></img>";
                else if ($row[1] == "docking station")
                    echo "<img src='../images/assets/dock.png' alt='' border=2 height=80 width=100></img>";
                else
                    echo "<img src='../images/assets/missing.png' alt='' border=2 height=100 width=100></img>";
                echo "</td>\n";
                echo "</tr>\n";
            }


            ?>
        </table>
    </div>

    <div id="PersonalInfo">
        <p> Personal information </p>
		<?php
		if ($_SESSION["employee"] == 1)
        	echo "<img class='photo' src='../images/admin_photo.jpeg' height='150' width='300'>";
		else if ($_SESSION["employee"] == 2)
        	echo "<img class='photo' src='../images/persons/person1.jpeg' height='150' width='300'>";
		else 
        	echo "<img class='photo' src='../images/admin_photo.jpeg' height='150' width='300'>";
		?>
		<table>
            <?php
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
            while ($row = $result->fetch_row()) {
                $f_name = $row[0];
                $l_name = $row[1];
                $room_name = $row[2];
            }

            echo "<tr> <th> Name </th> <td class='table_cell'>$f_name $l_name</td> </tr>";
            echo "<tr> <th> Position </th> <td class='table_cell'>$pos</td> </tr>";
            echo "<tr onclick='show_map_modal();' onmouseover='' style='cursor: pointer;'> <th>Working place</th> <td class='table_cell'>$room_name</td> </tr>";
            ?>
        </table>
    </div>
    <div id="Report">
        <p>Please report your issue</p>
        <form action="<?= $_SERVER["PHP_SELF"] ?>" method="post">
            <textarea id="reportTextArea" rows="5" style="width: 80%;" name="issue"></textarea>
            <input type="submit" name="action" onclick="sendRepor()" value="Send report"/>
        </form>

        <?php
        if (isset($_POST["action"])&&(strlen(trim($_POST['issue']))))
            !mysqli_query($db, "INSERT INTO employeeReport(report_text, date ,id_employee) VALUES ('{$_POST["issue"]}', now(),'{$_SESSION["employee"]}');");

         ?>
    </div>


</div>
<div id="map" class="modal">
    <div id="map_form" class="map_inside">
        <div class="close" onclick="close_map_modal();" onmouseover='' style='cursor: pointer;'><img
                    src="../images/icons/close.png"></div>
        <div class="form_olab"><?php echo $room_name; ?></div>
        <br>
        <div class="image_and_desc">
            <img id="map_image" class="photo" src="../images/rooms/1.png" height=500 width=750 border="2">
        </div>
    </div>
</div>


</body>

<script>
    function sendRepor() {
        if ($.trim($("#reportTextArea").val()) != "") {
            alert('report successfully sent');
        }else{
            alert('please, fill out your report')
        }


    }
</script>


</html>

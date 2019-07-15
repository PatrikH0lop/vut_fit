<html>
<?php
$db = mysqli_init();
if (!(mysqli_real_connect($db, 'localhost', '[REDACTED]', '[REDACTED]', '[REDACTED]', 0, '/var/run/mysql/mysql.sock'))) {
    die('cannot connect ' . mysqli_connecterror());
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
    <meta charset='utf-8'/>
    <meta content='en' name='language'/>
    <meta content='width=device-width, initial-scale=1' name='viewport'/>
    <link href="http://www.jqueryscript.net/css/jquerysctipttop.css" rel="stylesheet" type="text/css">
    <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.11.0/jquery.min.js"></script>
    <script src='multifilter.js'></script>
</head>
<script type='text/javascript'>
    //<![CDATA[
    $(document).ready(function () {
        $('.filter').multifilter()
    })
    //]]>
</script>


<body>
<div id="Options">
    <a href="../index.php">Logout</a>
    &nbsp&nbsp&nbsp
    <a href="admin.php">Home</a>
    &nbsp&nbsp&nbsp
    <a href="../user/user.php">Switch to user</a>
    &nbsp&nbsp&nbsp
    <a href="placeholder.php">placeholder</a>
</div>


<div id="MainPlaceholder">
    <div class='container'>
        <div class='filters'>
            <div id="MainPlaceholderFilter">
                Asset filter:
                <div class='filter-container'>
                    <input autocomplete='off' class='filter' name='name_of_asset' placeholder='Name of asset'
                           data-col='name of asset'/>
                </div>
                <div class='filter-container'>
                    <input autocomplete='off' class='filter' name='asset_desctiption' placeholder='Asset description'
                           data-col='asset description'/>
                </div>
                <div class='filter-container'>
                    <input autocomplete='off' class='filter' name='price' placeholder='price' data-col='price'/>
                </div>
                <div class='filter-container'>
                    <input autocomplete='off' class='filter' name='date_of_evidence' placeholder='date of evidence'
                           data-col='date of evidence'/>
                </div>
            </div>
            <div id="MainPlaceholderFilter">
                User filter:
                <div class='filter-container'>
                    <input autocomplete='off' class='filter' name='name' placeholder='name' data-col='first name'/>
                </div>
                <div class='filter-container'>
                    <input autocomplete='off' class='filter' name='lastname' placeholder='lastname'
                           data-col='lastname'/>
                </div>
                <div class='filter-container'>
                    <input autocomplete='off' class='filter' name='login' placeholder='login' data-col='login'/>
                </div>
            </div>
            <div id="MainPlaceholderFilter">
                Place filter:
                <div class='filter-container'>
                    <input autocomplete='off' class='filter' name='building' placeholder='building'
                           data-col='building'/>
                </div>
                <div class='filter-container'>
                    <input autocomplete='off' class='filter' name='room' placeholder='room' data-col='room'/>
                </div>
                <div class='filter-container'>
                    <input autocomplete='off' class='filter' name='floor' placeholder='floor' data-col='floor'/>
                </div>
            </div>
            <div class='clearfix'></div>
        </div>
    </div>
</div>

<div class='container'>
    <table id = 'dsTable'>
        <form action="<?=$_SERVER["PHP_SELF"]?>" method="post" name="assign_form">
        <thead>
        <th>image</th>
        <th>name of asset</th>
        <th>asset description</th>
        <th>price</th>
        <th>date of evidence</th>
        <th>first name</th>
        <th>lastname</th>
        <th>login</th>
        <th>building</th>
        <th>room</th>
        <th>floor</th>
        <th></th>
        </thead>
        <tbody>
        <?php

        $sql = "SELECT type , asset.name, asset.description, price, date_of_evidence, first_name, last_name, login, building, room.name, floor
                FROM asset
                LEFT JOIN employee ON asset.id_employee = employee.id
                LEFT JOIN room ON asset.id_room = room.id;";
        $result = mysqli_query($db, $sql);
        echo "<tr>";
        if ($result != false) {
            while ($row = $result->fetch_row()) {

                echo "<td id='asset-img'>\n";
                if ($row[0] == "mouse")
                    echo "<img style='margin-left: 40px' src='../images/assets/mouse.png' alt='' border=2 height=80 width=100></img>";
                else if ($row[0] == "keyboard")
                    echo "<img  style='margin-left: 40px' src='../images/assets/keyboard.png' alt='' border=2 height=80 width=100></img>";
                else if ($row[0] == "notebook")
                    echo "<img  style='margin-left: 40px' src='../images/assets/notebook.png' alt='' border=2 height=80 width=100></img>";
                else if ($row[0] == "monitor")
                    echo "<img style='margin-left: 40px'  src='../images/assets/monitor.png' alt='' border=2 height=80 width=100></img>";
                else if ($row[0] == "GPU")
                    echo "<img  style='margin-left: 40px' src='../images/assets/GPU.png' alt='' border=2 height=80 width=100></img>";
                else if ($row[0] == "docking station")
                    echo "<img style='margin-left: 40px' src='../images/assets/dock.png' alt='' border=2 height=80 width=100></img>";
                else
                    echo "<img  style='margin-left: 40px' src='../images/assets/missing.png' alt='' border=2 height=100 width=100></img>";
                echo "</td>\n";


                printf("<td>%s</td>\n<td>%s</td>\n<td>%s</td>\n<td>%s</td>\n<td>%s</td>\n<td>%s</td>\n<td>%s</td>\n<td>%s</td>\n<td>%s</td>\n<td>%s</td>\n<td>%s</td>\n",
                    $row[1], $row[2], $row[3], $row[4], $row[5], $row[6], $row[7], $row[8], $row[9], $row[10], "<input type=\"button\" class=\"button-asset\" value=\"Delete\" onclick = \"deleteRow(this)\"");



                echo "</tr>\n";

            }

        } else {
            echo "<h2>Nebyly nalezenny žádné záznamy</h2>";
        }
        ?>
        </tbody>
        </form>
    </table>

<?php
if (isset($_POST["assign_room"])) {
    $sql_command = "DELETE FROM asset WHERE id=$add_equipment;";
    if (!mysqli_query($db, $sql_command)) {
        //echo mysqli_error($db);
    }
}

?>

</div>
</body>
<script type="text/javascript">

    var _gaq = _gaq || [];
    _gaq.push(['_setAccount', 'UA-36251023-1']);
    _gaq.push(['_setDomainName', 'jqueryscript.net']);
    _gaq.push(['_trackPageview']);

    (function () {
        var ga = document.createElement('script');
        ga.type = 'text/javascript';
        ga.async = true;
        ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
        var s = document.getElementsByTagName('script')[0];
        s.parentNode.insertBefore(ga, s);
    })();


    function deleteRow(row){
        var d = row.parentNode.parentNode.rowIndex;
        document.getElementById('dsTable').deleteRow(d);
    }
</script>
</html>

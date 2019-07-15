<?php

include 'dbh.php';

$id_asset = htmlspecialchars($_GET['name']);
$sql_command = "SELECT type, name, price, description, date_of_evidence FROM asset WHERE id = $id_asset";
$result = mysqli_query($db, $sql_command);
if ($result !== false) {
    while ($row = $result->fetch_row()) {
        echo json_encode($row);
    }

}

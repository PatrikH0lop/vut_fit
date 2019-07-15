<?php

$db = mysqli_init();
if (!mysqli_real_connect($db, 'localhost', '[REDACTED]', '[REDACTED]', '[REDACTED]', 0, '/var/run/mysql/mysql.sock')) {
    die('cannot connect '.mysqli_connecterror());
}

?>

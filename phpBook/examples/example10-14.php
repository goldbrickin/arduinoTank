<?php
  require_once 'login.php';
  $db_server = mysql_connect($db_hostname, $db_username, $db_password);

  if (!$db_server) die("Unable to connect to MySQL: " . mysql_error());

  mysql_select_db($db_database)
    or die("Unable to select database: " . mysql_error());

  $query  = "UPDATE cats SET name='Charlie' WHERE name='Charly'";
  $result = mysql_query($query);

  if (!$result) die ("Database access failed: " . mysql_error());
?>
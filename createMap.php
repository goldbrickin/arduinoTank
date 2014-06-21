<?php
require_once 'login.php';
$db_server = mysql_connect($db_hostname, $db_username, $db_password);
if (!$db_server) die("Unable to connect to MySQL: " . mysql_error());
mysql_select_db($db_database)
or die("Unable to select database: " . mysql_error());

$mapString = "";
$mapString = createMap("full");
saveMap($mapString);
echo getLastMap();

function getLastMap()
{
    $map = "";
    $query = "SELECT grid FROM map ORDER BY createdAt DESC LIMIT 1";

    $result = mysql_query($query);
    if (!$result) die("Database access failed: " . mysql_error());

    $rows = mysql_num_rows($result);

    if ($rows > 0)
    {
        $row = mysql_fetch_row($result);
        $map = $row[0];
    }

    return $map;
}

function saveMap($map)
{
    $query = "INSERT INTO map (grid) VALUES ('$map')";

    $result = mysql_query($query);
    if (!$result) die("Database access failed: " . mysql_error());
}

function createMap($mapType)
{
    $map = '';

    for ($i = 1; $i <= 961; $i++)
        $map .= $mapType == "full" ? 1 : 0;

    return $map;
}


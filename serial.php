<?php
error_reporting(E_ALL);
ini_set('display_errors', '1');
require "PhpSerial.php";

$serial = new phpSerial;
$serial->deviceSet("/dev/ttyAMA0");
$serial->confBaudRate(9600);
$serial->confParity("none");
$serial->confCharacterLength(8);
$serial->confStopBits(1);
$serial->confFlowControl("none");
$serial->deviceOpen();
$serial->sendMessage("d");
$returnData = $serial->readPort();
$serial->deviceClose();


echo $returnData;

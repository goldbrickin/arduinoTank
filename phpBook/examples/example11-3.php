<?php
  $query  = "SELECT * FROM classics";
  $result = $connection->query($query);

  if (!$result) die($connection->error);
?>
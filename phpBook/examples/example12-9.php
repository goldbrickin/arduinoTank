<?php
  function sanitizeString($var)
  {
    if (get_magic_quotes_gpc())
      $var = stripslashes($var);
    $var = htmlentities($var);
    $var = strip_tags($var);
    return $var;
  }

  function sanitizeMySQL($connection, $var)
  { // Using the mysqli extension
    $var = $connection->real_escape_string($var);
    $var = sanitizeString($var);
    return $var;
  }
?>
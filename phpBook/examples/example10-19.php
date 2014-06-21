<?php
  $user  = mysql_fix_string($_POST['user']);
  $pass  = mysql_fix_string($_POST['pass']);
  $query = "SELECT * FROM users WHERE user='$user' AND pass='$pass'";

  function mysql_fix_string($string)
  {
    if (get_magic_quotes_gpc()) $string = stripslashes($string);
    return mysql_real_escape_string($string);
  }
?>
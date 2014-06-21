<?php
  function mysql_entities_fix_string($string)
  {
    return htmlentities(mysql_fix_string($string));
  }	

  function mysql_fix_string($string)
  {
    if (get_magic_quotes_gpc()) $string = stripslashes($string);
    return mysql_real_escape_string($string);
  }
?>

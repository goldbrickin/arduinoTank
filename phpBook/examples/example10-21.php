<?php
  require 'login.php';
  $db_server = mysql_connect($db_hostname, $db_username, $db_password);

  if (!$db_server) die("Unable to connect to MySQL: " . mysql_error());

  mysql_select_db($db_database)
  	or die("Unable to select database: " . mysql_error());

  $query = 'PREPARE statement FROM "INSERT INTO classics VALUES(?,?,?,?,?)"';
  mysql_query($query);

  $query = 'SET @author = "Emily Brontë",' .
  		 '@title = "Wuthering Heights",' .
  		 '@category = "Classic Fiction",' .
  		 '@year = "1847",' .
  		 '@isbn = "9780553212587"';
  mysql_query($query);

  $query = 'EXECUTE statement USING @author,@title,@category,@year,@isbn';
  mysql_query($query);

  $query = 'DEALLOCATE PREPARE statement';
  mysql_query($query);
?>

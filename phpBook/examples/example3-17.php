<?php
  function test()
  {
    static $count = 0;
    echo $count;
    $count++;
  }

  for ($i = 1; $i < 11; $i++)
{
    test();
}

?>

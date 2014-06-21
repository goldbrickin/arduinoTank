<?php
echo "<html><head><title>Map</title>";
echo "<script type='text/javascript' src='js/map.js'></script>";
echo "</head><body>";

echo "<div id='mapGrid'>";

for ($i = 1; $i <= 31; $i++)
{
    for ($j = 1; $j <= 31; $j++)
    {
        echo "<div id='" . $i . "." . $j . "' style='position:absolute;left:" . 30*($j-1) . "px;top:" . 30*($i-1)
            . "px;background-color:orange;width:30px;height:30px;' onclick='clickCell(\"" . $i . "." . $j . "\")'></div>";
    }
}

echo "</div>";

echo "</body></html>";

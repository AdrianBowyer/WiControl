<?php

if (isset($HTTP_RAW_POST_DATA))
{
   file_put_contents("../../sandb-profile-thursday.dat", $HTTP_RAW_POST_DATA);
   echo('Saved');
}
?>

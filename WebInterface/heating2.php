<!DOCTYPE HTML>
<html>
<body style="background-color: rgb(225,225,225)">


<?php
$infile = file_get_contents('../workshop-profile.dat', FILE_USE_INCLUDE_PATH);
?>

West End Stables: Workshop Heating Settings
<br>

<form name="savefile" method="post" action=""><br/>
<textarea rows="16" cols="100" name="textdata">
<?php echo $infile; ?>
</textarea><br/>
<input type="submit" name="submitsave" value="Save heating settings">
</form>

<br/>

<?php
    if (isset($_POST))
	 {
        if ($_POST['submitsave'] == "Save heating settings") 
		  {
            $text = $_POST["textdata"];
            file_put_contents("../workshop-profile.dat", $old . $text);
        }
    }
?>
</body>
</html>

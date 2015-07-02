<?php

if(isset($_POST['submit'])){

	//collect form data
	$q1 = $_POST['q1'];
	$q2 = $_POST['q2'];
	$q3 = $_POST['q3'];
	$q4 = $_POST['q4'];
	$q5 = $_POST['q5'];
	$q6 = $_POST['q6'];
	$q7 = $_POST['q7'];
	    
 
	# Title of the CSV
	$Content = $q1.",".$q2.",".$q3.",".$q4.",".$q5.",".$q6.",".$q7."\n";

	$fp = fopen("/var/www/feedback/feedback-".date("d-m-y-h:i:s").".csv","a"); // $fp is now the file pointer to file $filename

	if($fp){
		fwrite($fp,$Content); // Write information to the file
		fclose($fp); // Close the file
	}
	header("Location: ./goodbye.html");
	exit();
    
 }
?>

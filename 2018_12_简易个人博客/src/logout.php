<?php
	include_once("function/util.php");

	session_start();
	if(!empty($_SESSION['UserName'])){
		//删除session值，使其为未登录状态
		unset($_SESSION['UserName']);
	}

	echo "Logout! <br>";
	echo "Trun to login.html in 3 second<br>";

	auto_jump("login.html");

?>
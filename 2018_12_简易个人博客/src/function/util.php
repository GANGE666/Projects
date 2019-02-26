<?php

	function auto_jump($target_page)
	{
    	echo "<script language=\"javascript\" type=\"text/javascript\"> setTimeout(\"javascript:location.href='$target_page'\", 2000); </script>";

    	return;
	}

	//判断用户是否登陆
	function islogin()
	{
		session_start();
		if(empty($_SESSION['UserName'])){
			auto_jump("login.html");
			exit("Please Login!\nJump to login.html in 3 second!");
		}
		else if($_SESSION['UserName'] == "admin"){
			echo "User: ".$_SESSION['UserName']."<br>";
			return $_SESSION['UserName'];
		}
		else{
			echo "User: ".$_SESSION['UserName']."<br>";
			return $_SESSION['UserName'];
		}
	}

	//判断是否为管理员
	function isadmin()
	{
		session_start();
		if(empty($_SESSION['UserName'])){
			auto_jump("login.html");
			exit("Please Login!\nJump to login.html in 3 second!");
		}
		else if($_SESSION['UserName'] == "admin"){
			return 1;
		}
		else{
			return 0;
		}
	}

	function only_admin()
	{
		session_start();
		if(empty($_SESSION['UserName'])){
			auto_jump("login.html");
			exit("Please Login!\nJump to login.html in 3 second!");
		}
		else if($_SESSION['UserName'] == "admin"){
			return 1;
		}
		else{
			echo "You are not admin!<br>";
			auto_jump("index.php");
			exit();
		}
	}




?>
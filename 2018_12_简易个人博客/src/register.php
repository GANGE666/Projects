<?php
    include_once("function/database.php");
    include_once("function/util.php");

    if (empty($_POST)) {
        exit("Nothing Post! <br>");
    }

    // 判断输入密码与确认密码是否相同
    $password = addslashes($_POST['password']);
    $confirmPassword = addslashes($_POST['confirmPassword']);
    if ($password != $confirmPassword) {
        exit("输入的密码与确认密码不相等！");
    }

    $userName = addslashes($_POST['userName']);

    // 判断用户名是否重复
    $userNameSQL = "select * from users where UserName = '$userName'";
    getConnect();
    $resultSet = mysql_query($userNameSQL);
    if (mysql_num_rows($resultSet) > 0) {
        exit("用户名已被占用，请更换其他用户名");
    }

    $sex = addslashes($_POST['sex']);
    $interests = addslashes($_POST['remark']);

    $registerSQL = "insert into users values(null, '$userName', '$password', '$sex', '$remark')";

    echo $registerSQL."<br>";

    mysqli_query($conn, "SET AUTOCOMMIT=0"); // 设置为不自动提交，因为MYSQL默认立即执行
    mysqli_begin_transaction($conn);            // 开始事务定义

    if(!mysql_query($registerSQL))
    {
        mysqli_query($conn, "ROLLBACK");     // 判断当执行失败时回滚
    }
    mysqli_commit($conn);            //执行事务

    $userID = mysql_insert_id();
    echo "注册成功<br>";

    $userSQL = "select * from users where user_id = '$userID'";
    $userResult = mysql_query($userSQL);
    if ($user = mysql_fetch_array($userResult)) {
        echo "您的注册用户名为：" . $userName;
    } else {
        exit("用户注册失败！请重试");
        auto_jump("register.html");
    }
    closeConnect();

    //header('Location:index.html');
    echo "<script language=\"javascript\" type=\"text/javascript\"> setTimeout(\"javascript:location.href='login.html'\", 3000); </script>";
    exit;

?>

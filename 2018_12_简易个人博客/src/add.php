<?php
    include_once("function/util.php");
    $current_user = islogin();
    only_admin();
?>

<a href="index.php"><B>index</B></a>

<?php
    if($current_user == "admin")
        echo "<a href=\"add.php\"><B>add blog</B></a>";
?>

<a href="logout.php"><B>Logout</B></a>

<hr>

<?php

    include("conn.php"); //引入连接数据库

    if (!empty($_POST['sub'])) {    

        $title = addslashes($_POST['title']);  //获取title表单内容

        $con = addslashes($_POST['con']);      //获取contents表单内容

        $sql= "insert into blog values(null,'0','$title',now(),'$con')";     

        mysqli_query($conn, "SET AUTOCOMMIT=0"); // 设置为不自动提交，因为MYSQL默认立即执行
        mysqli_begin_transaction($conn);            // 开始事务定义

        if(!mysql_query($sql))
        {
            mysqli_query($conn, "ROLLBACK");     // 判断当执行失败时回滚
        }
        mysqli_commit($conn);            //执行事务

        echo "insert success!";

}?>

<form action="add.php" method="post">

    title   :<br>

    <input type="text" name="title"><br><br>

    contents:<br>

    <textarea rows="5" cols="50" name="con"></textarea><br><br>

    <input type="submit"  name="sub" value="submit">

</form>
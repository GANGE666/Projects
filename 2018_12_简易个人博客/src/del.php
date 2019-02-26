<?php
    include_once("function/util.php");
    islogin();
    only_admin();
?>

<a href="index.php"><B>index</B></a>

<a href="add.php"><B>add blog</B></a>

<a href="logout.php"><B>Logout</B></a>

<hr>

<?php    

    include("conn.php"); //引入连接数据库

    if (!empty($_GET['id'])) {        

             $del = addslashes($_GET['id']);  //删除blog

        $sql= "delete from blog where id='$del' ";        

        mysql_query($sql);        

        echo "delete success!";

}?>
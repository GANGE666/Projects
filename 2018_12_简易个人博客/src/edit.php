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

//获取数据库表数据
    if (!empty($_GET['id'])) {    

    $edit = $_GET['id'];    

    $sql = "select * from blog where id='$edit'";  
    //$sql .= "LIMIT 0,50 LOCK IN SHARE MODE";  

    $query = mysql_query($sql);    

    $rs = mysql_fetch_array($query);

    }
    //更新数据库表数据
    if (!empty($_POST['sub'])) {    

        $title = addslashes($_POST['title']);  //获取title表单内容

    $con = addslashes($_POST['con']);      //获取contents表单内容

    $hid = addslashes($_POST['hid']); 

    $sql= "update blog set title='$title', contents='$con' where id='$hid' ";
    //$sql .= "LIMIT 0,50 FOR UPDATE"  

    mysql_query($sql);    

    echo "<script>alert('update success.');location.href='index.php'</script>";

}?>

<form action="edit.php" method="post">

    <input type="hidden" name="hid" value="<?php echo $rs['id'];?>">

    title   :<br>

    <input type="text" name="title" value="<?php echo $rs['title'];?>">

    <br><br>

    contents:<br>

    <textarea rows="5" cols="50" name="con" ><?php echo $rs['contents'];?></textarea><br><br>

    <input type="submit"  name="sub" value="submit">

</form>
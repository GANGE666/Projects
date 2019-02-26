<?php
    include_once("function/database.php");
    include_once("function/util.php");

    session_start();
    if(!empty($_SESSION['UserName'])){
        echo "You have been login!<br>";
        auto_jump("index.php");
        exit();
    }


    // $userName = $_POST['userName'];
    // $password = $_POST['password'];
    $userName = addslashes($_POST['userName']);
    $password = addslashes($_POST['password']);
    getConnect();
    $loginSQL = "select * from users where UserName='$userName' and password='$password'";
    //echo $loginSQL;
    $resultLogin = mysql_query($loginSQL);

    $user = mysql_fetch_array($resultLogin);

    if (mysql_num_rows($resultLogin) > 0) {
        echo "登录成功";
        echo "<br> "."Welcome : ".$user['UserName'];
        set_session($user['UserName']);
        echo "<br>"."Turn to index.php in 3second";

        auto_jump("index.php");
    } else {
        echo "登录失败";

        auto_jump("login.html");
    }
    closeConnect();
?>

<?php
function set_session($UserName){
    //session_start();

    $_SESSION['UserName'] = $UserName;
    //echo $_SESSION['UserName'];

    return;
}

?>
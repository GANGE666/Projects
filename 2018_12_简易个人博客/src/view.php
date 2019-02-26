<!DOCTYPE html>
<html lang="en" >

<head>
  <meta charset="UTF-8">
  <title>My blog</title>
  <meta name="viewport" content="width=device-width">
<link rel="stylesheet" href="//maxcdn.bootstrapcdn.com/font-awesome/4.3.0/css/font-awesome.min.css">
<link href='https://fonts.googleapis.com/css?family=Lato:300,400,700|Poiret+One' rel='stylesheet' type='text/css'>
  
  
      <link rel="stylesheet" href="css/index_style.css">
      <?php
            include_once("function/util.php");
            $current_user = islogin();
        ?>
  
</head>

<body>

  <header>
          <h1 class="logo">Xxg's Blog</h1>
    </header>
    
    <nav>
            <ul>
                    <li class="nav-item"><a href="index.php" >Home</a></li>
                    <?php
                        if($current_user == "admin")
                            echo "<li class=\"nav-item\"><a href=\"add.php\">add blog</a></li>";
                    ?>

                    <li class="nav-item"><a href="logout.php">Logout</a></li>
              </ul>
                  <div class="menu-bar">Menu
                        <span class="hamburger-icon"><i class="fa fa-bars"></i></span>
              </div>
        </nav>
    
    <div class="container">
          <div class="section">
                <div class="col span_2_of_3">


                    <?php
                        include_once("conn.php"); //引入连接数据库
                        
                        if (!empty($_GET['id'])) {        

                        $id = addslashes($_GET['id']);        

                        $sql ="select * from blog  where id='$id' "; 

                        $query = mysql_query($sql);        

                        $rs = mysql_fetch_array($query);        

                        $sqlup = "update blog set hits=hits+1 where id='$id'";        

                        mysql_query($sqlup);    
                                        
                    ?>
                    <div class="blog-post">
                        <h1 class="blog-title">
                            <a href="view.php?id=<?php echo $rs['id']; ?>"><?php echo $rs['title']; ?></a></h1>
                        
                        <h2 class="date">Posted <?php echo $rs['date']; ?></h2>
                    
                    <p class="blog-content"><?php echo $rs['contents']; ?></p>  
                                       
                    </div>
                    <?php
                    };?>
                </div>
                
                <aside class="col span_1_of_3">
                      <div class="side-post">
                            Search Title
                            <form action="index.php" method="get" style='align:"right"'>
                                <input type="text" name="keys" >
                                <input class="button" type="submit" name="subs" value="Search">
                             </form>
                      </div>
                
                   
                 </aside>
          </div>
    </div>
  <script src='http://cdnjs.cloudflare.com/ajax/libs/jquery/2.1.3/jquery.min.js'></script>

  

    <script  src="js/index.js"></script>




</body>

</html>
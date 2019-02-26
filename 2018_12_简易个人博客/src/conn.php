<?php

    @mysql_connect("127.0.0.1:3306","root","fh834t6dgf439dhcbjds8723") or die("mysql数据库连接失败");
    mysql_query("set names utf-8");
    @mysql_select_db("test")or die("db连接失败");

?>
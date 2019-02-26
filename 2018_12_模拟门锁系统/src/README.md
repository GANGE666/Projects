文件采用相对路径，所以所有操作须在本目录下完成

1、在mylock文件夹及lock_guard文件夹中make成功后，进入主文件夹：
2、执行install.sh安装相关驱动
3、执行init.sh将mylock文件copy覆盖其备份（否则启动lock_guard会将新生成的elf文件误认为是发生错误的elf）
4、在主文件夹下执行 ./lock_guard/lock_guard启动监控进程，监控进程会启动mylock或使用脚本./run.sh

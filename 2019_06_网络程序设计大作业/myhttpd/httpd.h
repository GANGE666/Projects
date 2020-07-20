#ifndef __HTTPD_H__
#define __HTTPD_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h> 
#include <netdb.h>  
#include <pthread.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h> 
#include <unistd.h> 
#include <dirent.h> 
#include <limits.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <stddef.h>
#include <semaphore.h>
#include <sys/mman.h>




#define ISspace(x) isspace((int)(x))

#define SERVER_STRING "Server: MyHTTPServer\r\n"

void accept_request(void *);
void bad_request(int);
void cat(int, FILE *);
void cannot_execute(int);
void error_die(const char *);
void execute_cgi(int, const char *, const char *, const char *, char *);
int get_line(int, char *, int, char *);
void headers(int, const char *);
void not_found(int);
void serve_file(int, const char *, char *);
int startup(u_short *);
void unimplemented(int);

// 配置文件相关
#define FILE_LINELENGTH 100
#define DEFAULT_Conf_File_Path "./myhttpd.conf"
#define DEFAULT_Log_File_Path "./myhttpd.log"

struct conf_opts{
	char CGIRoot[128];		/*CGI跟路径*/
	char DefaultFile[128];		/*默认文件名称*/
	char DocumentRoot[128];	/*根文件路径*/
	char ConfigFile[128];		/*配置文件路径和名称*/
    char LogFile[128];       // 日志文件
	int ListenPort;			/*侦听端口*/
	int MaxClient;			/*最大客户端数量*/
	int TimeOut;				/*超时时间*/
	int InitClient;				/*初始化线程数量*/
};
struct conf_opts conf_para;

//pthread 传参结构体
struct thread_args{
    int client_sock;
    struct sockaddr_in client_name;
};

struct mine_type{
	char	*extension;
	int 			type;
	int			ext_len;
	char	*mime_type;
};

#endif



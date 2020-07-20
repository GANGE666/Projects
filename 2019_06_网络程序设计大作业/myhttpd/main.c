#include "httpd.h"

static     int server_sock = -1;        //  仅本文件内使用
static     int client_sock = -1;


#define MY_TEST



void test(){
}

void socket_release(){
    close(server_sock);
    close(client_sock);
}

void ReleaseAll(){
    log_release();
    socket_release();
    printf("\n\nExit httpd server! Resources released!\n\n");
    _exit(0);
}

void InitAll(int argc, char *argv[]){
    Para_Init(argc,argv);       // 解析配置
    log_init();     // log 初始化

    atexit(ReleaseAll);     //析构函数初始化
    signal(SIGINT, ReleaseAll); // ctrl c 注册
}

int startup(u_short *port)
{
 int httpd = 0;
 struct sockaddr_in name;

 httpd = socket(PF_INET, SOCK_STREAM, 0);
 if (httpd == -1)
  error_die("socket");
 memset(&name, 0, sizeof(name));
 name.sin_family = AF_INET;
 name.sin_port = htons(*port);
 name.sin_addr.s_addr = htonl(INADDR_ANY);
 if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
  error_die("bind");
 if (*port == 0)  /* if dynamically allocating a port */
 {
  int namelen = sizeof(name);
  if (getsockname(httpd, (struct sockaddr *)&name, &namelen) == -1)
   error_die("getsockname");
  *port = ntohs(name.sin_port);
 }
 if (listen(httpd, 5) < 0)
  error_die("listen");
 return(httpd);
}


int main(int argc, char *argv[])
{
    u_short port = 0;
    struct sockaddr_in client_name;
    int client_name_len = sizeof(client_name);
    pthread_t newthread;
    struct thread_args * tmp_thread_args;

    InitAll(argc,argv);

    #ifdef MY_TEST
    //test();
    #endif

    server_sock = startup(&conf_para.ListenPort);
    printf("httpd running on port %d\n", conf_para.ListenPort);

    while (1)
    {
        client_sock = accept(server_sock,
                            (struct sockaddr *)&client_name,
                            &client_name_len);
        if (client_sock == -1)
            error_die("accept");

        tmp_thread_args = (struct thread_args *)malloc(sizeof(struct thread_args));
        tmp_thread_args->client_sock = client_sock;
        void * tmp1 = &(tmp_thread_args->client_name);
        void * tmp2 = &(client_name);
        memcpy(tmp1, tmp2, sizeof(struct sockaddr_in));

        if (pthread_create(&newthread , NULL, accept_request, tmp_thread_args) != 0)
            perror("pthread_create error");
    }

    close(server_sock);

    return(0);
}
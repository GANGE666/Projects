#include "httpd.h"


void cat(int client, FILE *resource)
{
 char buf[1024];

 fgets(buf, sizeof(buf), resource);
 while (!feof(resource))
 {
  send(client, buf, strlen(buf), 0);
  fgets(buf, sizeof(buf), resource);
 }
}

ssize_t rio_writen(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n; // 剩下的未写入字节数
    ssize_t nwritten;
    char *bufp = (char *)usrbuf;

    while (nleft > 0) {
        if ((nwritten = write(fd, bufp, nleft)) <= 0) {
            if (errno == EINTR) { // 被信号处理函数中断返回
                nwritten = 0;
            } else { // write函数出错
                return -1;
            }
        }
        nleft -= nwritten;
        bufp += nwritten;
    }

    return n;
}

void write_to_client(int fd, char * filename){
  char *srcp;
  int filesize = file_size(filename);
  int srcfd;

  if ((srcfd = open(filename, O_RDONLY, 0)) < 0) {
        printf("open file error");
    }

    if ((srcp = mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0)) == ((void *) -1)) {
        printf("mmap error");
    }
    close(srcfd);
    if (rio_writen(fd, srcp, filesize) < 0) {
        printf("wirte to client error");
    }

    if (munmap(srcp, filesize) < 0) {
        printf("munmap error");
    }
}



int get_line(int sock, char *buf, int size, char *record)
{
 int i = 0;
 char c = '\0';
 int n;

 while ((i < size - 1) && (c != '\n'))
 {
  n = recv(sock, &c, 1, 0);
  /* DEBUG printf("%02X\n", c); */
  if (n > 0)
  {
   if (c == '\r')
   {
    n = recv(sock, &c, 1, MSG_PEEK);
    /* DEBUG printf("%02X\n", c); */
    if ((n > 0) && (c == '\n'))
     recv(sock, &c, 1, 0);
    else
     c = '\n';
   }
   buf[i] = c;
   i++;
  }
  else
   c = '\n';
 }
 buf[i] = '\0';
  //printf("get_line: %s", buf);
  strcat(record, buf);
 return(i);
}



void headers(int client, const char *filename)
{
 char buf[1024];
 (void)filename;  /* could use filename to determine file type */

 struct mine_type *mine = NULL;
 mine = Mine_Type(filename, strlen(filename));
 printf("\nmine: %s\n", mine->mime_type);

 strcpy(buf, "HTTP/1.1 200 OK\r\n");
 send(client, buf, strlen(buf), 0);
 strcpy(buf, SERVER_STRING);
 send(client, buf, strlen(buf), 0);
 //sprintf(buf, "Accept-Ranges: bytes\r\n");
 //send(client, buf, strlen(buf), 0);
 sprintf(buf, "Content-Length: %lu\r\n", file_size(filename));
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "Content-Type: %s\r\n", mine->mime_type);
 send(client, buf, strlen(buf), 0);
 strcpy(buf, "\r\n");
 send(client, buf, strlen(buf), 0);

//printf("\nlen: %d\n", file_size(filename));

}



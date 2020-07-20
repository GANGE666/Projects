#include"httpd.h"


void accept_request(void * arg)
{
  int client;

  struct thread_args * tmp_thread_args;
  tmp_thread_args = (struct thread_args * )arg;
  client = tmp_thread_args->client_sock;
  struct sockaddr_in client_name;
  client_name = tmp_thread_args->client_name;

  printf("clinet connet ip %s!\n",inet_ntoa(client_name.sin_addr));

  char record[1024*1024];
  memset(record, 0, sizeof(record));
  get_time(record);
  strcat(record, inet_ntoa(client_name.sin_addr));

  char buf[1024];
  int numchars;
  char method[255];
  char url[255];
  char path[512];
  size_t i, j;
  struct stat st;
  int cgi = 0;      /* becomes true if server decides this is a CGI
                    * program */
  char *query_string = NULL;

  numchars = get_line(client, buf, sizeof(buf), record);
  //printf("get_line: %s", buf);
  //snprintf(record, sizeof(buf), "%s %s", record, buf);

  i = 0; j = 0;
  while (!ISspace(buf[j]) && (i < sizeof(method) - 1))
  {
    method[i] = buf[j];
    i++; j++;
  }
  method[i] = '\0';

  if (strcasecmp(method, "GET") && strcasecmp(method, "POST"))
  {
    unimplemented(client);
    return;
  }

  if (strcasecmp(method, "POST") == 0)
    cgi = 1;

  i = 0;
  while (ISspace(buf[j]) && (j < sizeof(buf)))
    j++;
  while (!ISspace(buf[j]) && (i < sizeof(url) - 1) && (j < sizeof(buf)))
  {
    url[i] = buf[j];
    i++; j++;
  }
  url[i] = '\0';

  if (strcasecmp(method, "GET") == 0)
  {
    query_string = url;
    while ((*query_string != '?') && (*query_string != '\0'))
        query_string++;
    if (*query_string == '?')
    {
      cgi = 1;
      *query_string = '\0';
      query_string++;
    }
  }

  sprintf(path, "%s%s",conf_para.DocumentRoot, url);
  if (path[strlen(path) - 1] == '/')
    strcat(path, "index.html");
  if (stat(path, &st) == -1) {
    while ((numchars > 0) && strcmp("\n", buf)){  /* read & discard headers */
      numchars = get_line(client, buf, sizeof(buf), record);
    //printf("get_line: %s", buf);
    //snprintf(record, sizeof(buf), "%s %s", record, buf);
    }
  not_found(client);
  }
  else
  {
  if ((st.st_mode & S_IFMT) == S_IFDIR)
    strcat(path, "/index.html");
  if ((st.st_mode & S_IXUSR) ||
      (st.st_mode & S_IXGRP) ||
      (st.st_mode & S_IXOTH)    )
    cgi = 1;
  if (!cgi)
    serve_file(client, path, record);
  else
    execute_cgi(client, path, method, query_string, record);
  }
  log_access(record);
  //printf("\nrecord:\n%s\n", record);
  close(client);
}


void execute_cgi(int client, const char *path,
                 const char *method, const char *query_string, char *record)
{
 char buf[1024];
 int cgi_output[2];
 int cgi_input[2];
 pid_t pid;
 int status;
 int i;
 char c;
 int numchars = 1;
 int content_length = -1;

 buf[0] = 'A'; buf[1] = '\0';
 if (strcasecmp(method, "GET") == 0)
  while ((numchars > 0) && strcmp("\n", buf)){  /* read & discard headers */
   numchars = get_line(client, buf, sizeof(buf), record);
  }
 else    /* POST */
 {
  numchars = get_line(client, buf, sizeof(buf), record);
  while ((numchars > 0) && strcmp("\n", buf))
  {
   buf[15] = '\0';
   if (strcasecmp(buf, "Content-Length:") == 0)
    content_length = atoi(&(buf[16]));
   numchars = get_line(client, buf, sizeof(buf), record);
  }
  if (content_length == -1) {
   bad_request(client);
   return;
  }
 }

 sprintf(buf, "HTTP/1.0 200 OK\r\n");
 send(client, buf, strlen(buf), 0);

 if (pipe(cgi_output) < 0) {
  cannot_execute(client);
  return;
 }
 if (pipe(cgi_input) < 0) {
  cannot_execute(client);
  return;
 }

 if ( (pid = fork()) < 0 ) {
  cannot_execute(client);
  return;
 }
 if (pid == 0)  /* child: CGI script */
 {
  char meth_env[255];
  char query_env[255];
  char length_env[255];

  dup2(cgi_output[1], 1);
  dup2(cgi_input[0], 0);
  close(cgi_output[0]);
  close(cgi_input[1]);
  sprintf(meth_env, "REQUEST_METHOD=%s", method);
  putenv(meth_env);
  if (strcasecmp(method, "GET") == 0) {
   sprintf(query_env, "QUERY_STRING=%s", query_string);
   putenv(query_env);
  }
  else {   /* POST */
   sprintf(length_env, "CONTENT_LENGTH=%d", content_length);
   putenv(length_env);
  }
  execl(path, path, NULL);
  exit(0);
 } else {    /* parent */
  close(cgi_output[1]);
  close(cgi_input[0]);
  if (strcasecmp(method, "POST") == 0)
   for (i = 0; i < content_length; i++) {
    recv(client, &c, 1, 0);
    write(cgi_input[1], &c, 1);
   }
  while (read(cgi_output[0], &c, 1) > 0)
   send(client, &c, 1, 0);

  close(cgi_output[0]);
  close(cgi_input[1]);
  waitpid(pid, &status, 0);
 }
}


void serve_file(int client, const char *filename, char *record)
{
 FILE *resource = NULL;
 int numchars = 1;
 char buf[1024];

 buf[0] = 'A'; buf[1] = '\0';
 while ((numchars > 0) && strcmp("\n", buf))  /* read & discard headers */
  numchars = get_line(client, buf, sizeof(buf), record);

 resource = fopen(filename, "r");
 if (resource == NULL)
  not_found(client);
 else
 {
  headers(client, filename);
  //cat(client, resource);
  write_to_client(client, filename);
 }
 fclose(resource);
}










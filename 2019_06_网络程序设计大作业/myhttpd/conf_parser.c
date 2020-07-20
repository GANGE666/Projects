#include "httpd.h"

static char *l_opt_arg;
static char *shortopts = "c:d:f:ho:l:m:t:";

static struct option longopts[] = {
	{"CGIRoot", 		required_argument, NULL, 'c'},
	{"ConfigFile", 		required_argument, NULL, 'f'},	
	{"DefaultFile", 	required_argument, NULL, 'd'},	
	{"DocumentRoot",	required_argument, NULL, 'o'},
	{"ListenPort", 		required_argument, NULL, 'l'},
	{"MaxClient", 		required_argument, NULL, 'm'},	
	{"TimeOut",		required_argument, NULL, 't'},
	{"Help",			no_argument, NULL, 'h'},
	{0, 0, 0, 0},
};

static int conf_readline(int fd , char *buff , int len)
{
	int n = -1 ; 
	int i = 0 ;
	int begin = 0 ;

	memset(buff , 0, len);
	for(i = 0 ; i < len ; i++)
	{
		n = read(fd , buff+i , 1);	
		if( n == 0)
		{
			*(buff+i) = '\0';
			break ;
		}
		else if(*(buff+i)=='\r' || *(buff+i)=='\n')
		{
			if(begin)
			{
				*(buff+i) = '\0';
				break;
			}
		}
		else
		{
			begin = 1 ;
		}
	}
	return i ;
}

static void display_usage(void)
{
	printf("sHTTPD -l number  -m number      -o path    -c  path   -d  filename   -t seconds  -o filename\n");
	printf("sHTTPD --ListenPort number\n");
	printf("       --MaxClient number\n");
	printf("       --DocumentRoot) path\n");
	printf("       --DefaultFile) filename\n");
	printf("       --CGIRoot path \n");
	printf("       --DefaultFile filename\n");
	printf("       --TimeOut seconds\n");
	printf("       --ConfigFile filename\n");
}

static int Para_CmdParse(int argc, char *argv[])
{
	int c;
	int len;
	int value;

	/*遍历输入参数,设置配置参数*/
	while ((c = getopt_long (argc, argv, shortopts, longopts, NULL)) != -1)
	{
		switch (c)
		{
			case 'c': 	/*CGI根路径*/
				l_opt_arg = optarg;
				if(l_opt_arg && l_opt_arg[0]!=':'){
					len = strlen(l_opt_arg);
					memcpy(conf_para.CGIRoot, l_opt_arg, len +1);
				}
				
				break;
			case 'd': 	/*默认文件名称*/
				l_opt_arg = optarg;
				if(l_opt_arg && l_opt_arg[0]!=':'){
					len = strlen(l_opt_arg);
					memcpy(conf_para.DefaultFile, l_opt_arg, len +1);
				}
				
				break;
			case 'f': 	/*配置文件名称和路径*/
				l_opt_arg = optarg;
				if(l_opt_arg && l_opt_arg[0]!=':'){
					len = strlen(l_opt_arg);
					memcpy(conf_para.ConfigFile, l_opt_arg, len +1);
				}
				
				break;
			case 'o': /*根文件路径*/
				l_opt_arg = optarg;
				if(l_opt_arg && l_opt_arg[0]!=':'){
					len = strlen(l_opt_arg);
					memcpy(conf_para.DocumentRoot, l_opt_arg, len +1);
				}
				
				break;
			case 'l': 	/*侦听端口*/
				l_opt_arg = optarg;
				if(l_opt_arg && l_opt_arg[0]!=':'){
					len = strlen(l_opt_arg);
					value = strtol(l_opt_arg, NULL, 10);
					if(value != LONG_MAX && value != LONG_MIN)
						conf_para.ListenPort = value;
				}
				
				break;
			case 'm': /*最大客户端数量*/
				l_opt_arg = optarg;
				if(l_opt_arg && l_opt_arg[0]!=':'){
					len = strlen(l_opt_arg);
					value = strtol(l_opt_arg, NULL, 10);
					if(value != LONG_MAX && value != LONG_MIN)
						conf_para.MaxClient= value;
				}				
				
				break;
			case 't': 	/*超时时间*/
				l_opt_arg = optarg;
				if(l_opt_arg && l_opt_arg[0]!=':'){
					printf("TIMEOUT\n");
					len = strlen(l_opt_arg);
					value = strtol(l_opt_arg, NULL, 10);
					if(value != LONG_MAX && value != LONG_MIN)
						conf_para.TimeOut = value;
				}
				
				break;
			case '?':/*错误参数*/
				printf("Invalid para\n");
			case 'h':	/*帮助*/
				display_usage();
                error_die("Please start httpd with correct para.\n");
				break;
		}
	}
	return 0;
}

void Parse_Conf(){
    char line[FILE_LINELENGTH];
    char *name = NULL, *value = NULL;
    int fd = -1;
    int n = 0;
    fd = open(conf_para.ConfigFile, O_RDONLY);
    if(fd == -1){
        dbg_printf("%s%s\n", "Cannot open config file: ", conf_para.ConfigFile);
        error_die("Please check config file.\n");
    }

	while( (n = conf_readline(fd, line, FILE_LINELENGTH)) !=0)
	{
        //dbg_printf("%s\n", line);
		char *pos = line;	
		/* 跳过一行开头部分的空格 */
		while(isspace(*pos)){
			pos++;
		}
		if(*pos == '#'){
			continue;
		}
		
		name = pos;
		while(!isspace(*pos) && *pos != '=')
		{
			pos++;
		}
		*pos = '\0';
        pos ++;
		while(*pos == ' ' || *pos == '=')
		{
			pos++;
		}
		value = pos;
		while(*pos != ';' && *pos != '\r' && *pos != '\n')
		{
			pos++;
		}
		*pos = '\0';/*生成值的字符串*/

        //dbg_printf("name\nvalue: %s\n", name, value);

		/*根据关键字部分，获得value部分的值*/
		int ivalue;
		if(strncmp("CGIRoot", name, 7) == 0)	{/*CGIRoot部分*/
			memcpy(conf_para.CGIRoot, value, strlen(value)+1);
		}else if(strncmp("DefaultFile", name, 11) == 0){/*DefaultFile部分*/
			memcpy(conf_para.DefaultFile, value, strlen(value)+1);
		}else if(strncmp("LogFile", name, 7) == 0){
			memcpy(conf_para.LogFile, value, strlen(value)+1);
		}else if(strncmp("DocumentRoot", name, 12) == 0){/*DocumentRoot部分*/
			memcpy(conf_para.DocumentRoot, value, strlen(value)+1);
		}else if(strncmp("ListenPort", name, 10) == 0){/*ListenPort部分*/
			ivalue = strtol(value, NULL, 10);
			conf_para.ListenPort = ivalue;
		}else if(strncmp("MaxClient", name, 9) == 0){/*MaxClient部分*/
			ivalue = strtol(value, NULL, 10);
			conf_para.MaxClient = ivalue;
		}else if(strncmp("TimeOut", name, 7) == 0){/*TimeOut部分*/
			ivalue = strtol(value, NULL, 10);
			conf_para.TimeOut = ivalue;
		}		
	}
	close(fd);




    return;
}


void display_para()
{
	printf("sHTTPD ListenPort: %d\n",conf_para.ListenPort);
	printf("       MaxClient: %d\n", conf_para.MaxClient);
	printf("       DocumentRoot: %s\n",conf_para.DocumentRoot);
	printf("       DefaultFile:%s\n",conf_para.DefaultFile);
	printf("       CGIRoot:%s \n",conf_para.CGIRoot);
	printf("       TimeOut:%d\n",conf_para.TimeOut);
	printf("       ConfigFile:%s\n",conf_para.ConfigFile);
	printf("       LogFile:%s\n",conf_para.LogFile);
}

void Para_Init(int argc, char *argv[])
{
    memcpy(conf_para.ConfigFile, DEFAULT_Conf_File_Path, strlen(DEFAULT_Conf_File_Path)+1); // 初始化配置文件路径
	memcpy(conf_para.LogFile, DEFAULT_Log_File_Path, strlen(DEFAULT_Log_File_Path)+1);

	/*解析命令行输入参数*/
	Para_CmdParse(argc, argv);
    // 可使用 ./httpd -f /home/test/myhttpd.conf 更改默认配置文件

	/*解析配置文件配置参数*/
	if(strlen(conf_para.ConfigFile))
		Parse_Conf();

	display_para();

	/*返回配置参数*/
	return ;
}


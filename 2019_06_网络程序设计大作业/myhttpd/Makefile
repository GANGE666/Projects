all: httpd

httpd: httpd.c main.c
	gcc -Wall -o httpd httpd.c http_misc.c http_log.c http_mine.c http_error.c conf_parser.c error_debug.c http_io.c main.c -lpthread

clean:
	rm httpd

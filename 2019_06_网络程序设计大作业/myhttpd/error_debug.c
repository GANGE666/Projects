#include <stdio.h>
#include <stdarg.h>

void error_die(const char *sc)
{
 perror(sc);
 exit(1);
}



void dbg_printf(char *format, ...)
{ 
	va_list ap;			
	char *p, *sval;
	int ival;
	double dval;
	va_start(ap, format);  //将ap指向第一个参数
	
	for( p = format; *p; p++)
	{
		if (*p != '%')
		{
			putchar(*p);
			continue;
		}
		else {
			switch(*++p){
				case 'd':
				{
					ival = va_arg(ap, int);
					printf("%d", ival);
					break;
				}
				case 'f': 
				{
					dval = va_arg(ap,double);
					printf("%f",dval);
					break;
				}
				case 's':
				{
					for(sval = va_arg(ap, char *); *sval; sval++)
					{
						putchar(*sval);
					}
					break;
				}
				
				default :{
					putchar(*p);
					break;
				}
			}
		}
	} 
	va_end(ap); 
	
}
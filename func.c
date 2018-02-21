#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

int str(char *str1, char *str2) /*Функция strcmp*/
{
	int i=0;
	while(str2[i]!='\0')
	{
		if(str1[i]==str2[i])
			i++;
		else
			return 1;
	}
return 0;
}

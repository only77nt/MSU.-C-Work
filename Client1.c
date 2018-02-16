#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 255
#define NAME_SIZE 30

int str(char *str1, char *str2)
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

int main() 
{
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	char buffer[BUFFER_SIZE];
	char Name[BUFFER_SIZE];
	char NameCpy[BUFFER_SIZE];
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(9734);

	len = sizeof(address);

	result = connect(sockfd, (struct sockaddr *)&address, len);
	if (result == -1) 
	{
		perror("oops : client1");
		exit(1);
	}

	printf("Enter your name: ");
	fgets(Name,BUFFER_SIZE-1,stdin);
	//printf("%s\n",Name);
	Name[strlen(Name)-1]=':';
	Name[strlen(Name)]=' ';
	strcpy(NameCpy,Name);
	//printf("%s!\n",NameCpy);
	
while(1)
{
		//printf("Enter your message: ");
		fgets(buffer, BUFFER_SIZE - 1, stdin); /*Вводится хорошо*/

		if(!str(buffer,"/exit") || !str(buffer,"/Exit")) /*Выход*/
		{
			printf("Have a good day! :)\n");
			memset(&buffer, 0, BUFFER_SIZE);
			memset(&Name, 0, BUFFER_SIZE); /*Чистим буффер*/
			strcpy(Name,NameCpy);
			close(sockfd);
			exit(0);
		}

		strncat(Name,buffer,BUFFER_SIZE-1);
		//printf("%s\n",Name);

		write(sockfd, Name, strlen(Name) - 1);
		memset(&Name, 0, BUFFER_SIZE); /*Чистим буффер*/
		memset(&buffer, 0, BUFFER_SIZE); /*Чистим буффер*/
		strcpy(Name,NameCpy);
		if (fork()==0)
	{
		while(1)
		{
			//printf("%d\n",sockfd);
			read(sockfd, Name, BUFFER_SIZE - 1);
			printf("%s\n",Name);
			memset(&Name, 0, BUFFER_SIZE); /*Чистим буффер*/
		}
	}
}
	
	close(sockfd);
	return 0;
}

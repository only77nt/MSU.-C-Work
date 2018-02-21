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
#include "sock.h"

#define BUFFER_SIZE 255 /*255 символов - стандартный размер буффера + 256 символ - '\0'*/
#define NAME_SIZE 30 /*30 символов вполне достаточно для никнейма*/

int main() 
{
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	char buffer[BUFFER_SIZE];
	char Name[BUFFER_SIZE];
	char NameCpy[BUFFER_SIZE];


	int server_sockfd, client_sockfd;
	int server_len, client_len;
	fd_set readfds, testfds;

	memset(&buffer, 0, BUFFER_SIZE); /*Чистим буффер*/	

	sockfd = socket(AF_INET, SOCK_STREAM, 0); /*Создаём сокет клиента*/

	address.sin_family = AF_INET; /*Настраиваем его*/
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(9734);

	len = sizeof(address);

	result = connect(sockfd, (struct sockaddr *)&address, len); /*Подключаемся к серверу*/
	if (result == -1) 
	{
		perror("oops : client1");
		exit(1);
	}

	printf("Enter your name: "); /*Запоминаем имя клиента*/
	memset(&Name, 0, BUFFER_SIZE); /*Чистим буффер*/
	fgets(Name,BUFFER_SIZE-1,stdin);
	Name[strlen(Name)-1]=':';
	Name[strlen(Name)]=' ';
	strcpy(NameCpy,Name);
	FD_ZERO(&readfds);

while(1)
{
		int max_d=sockfd;
		int fd;

		FD_SET(sockfd,&readfds);
		FD_SET(0,&readfds);

		result=select(max_d+1,&readfds,NULL,NULL,NULL); /*Ожидаем активность, организуем мультизадачность(одновременно читаем и пишем)*/
			if(FD_ISSET(sockfd,&readfds)) /*Если мы готовы что-то читать*/
			{
				read(sockfd, Name, BUFFER_SIZE - 1);
				printf("%s\n",Name);
				memset(&buffer, 0, BUFFER_SIZE); /*Чистим буффер*/
				memset(&Name, 0, BUFFER_SIZE); /*Чистим буффер*/
				strcpy(Name,NameCpy);
			}
			if(FD_ISSET(0,&readfds)) /*Если мы готовы что-то написать*/
			{
				memset(&buffer, 0, BUFFER_SIZE); /*Чистим буффер*/
				fgets(buffer, BUFFER_SIZE - 1, stdin); /*Вводится хорошо*/
				buffer[strlen(buffer)]='\0';

				if(!str(buffer,"/exit") || !str(buffer,"/Exit") || !str(buffer,"/EXIT")) /*Выход*/
				{
					printf("Have a good day! :)\n");
					memset(&buffer, 0, BUFFER_SIZE);
					memset(&Name, 0, BUFFER_SIZE); /*Чистим буффер*/
					strcpy(Name,NameCpy);
					close(sockfd);
					exit(0);
				}

				if(!str(buffer,"/help")) /*Помощь*/
				{
					printf("### This chat created by Korotkov Boris, Group 209 © ###\n");
					printf("### 1) To leave a chat write: /exit ###\n");
					printf("### 2) To change your nickname write: /nick ###\n");
					printf("### This command has a bag, the first client of the server can`t do it ###\n");
					printf("### 3) If you want to know who is online, write: /users ###\n");
					printf("### 4) To clear screen write: /clear");
					continue;
				}

				if(!str(buffer,"/clear")) /*Чистим экран*/
				{
					system("clear");
					continue;
				}

				if(!str(buffer,"/nick")) /*Меняем никнейм*/
				{
					write(sockfd, buffer, strlen(buffer) - 1);
					sleep(2);
					write(sockfd, NameCpy, strlen(NameCpy) - 1);
					printf("Enter your name: ");
					memset(&Name, 0, BUFFER_SIZE); /*Чистим буффер*/
					fgets(Name,BUFFER_SIZE-1,stdin);
					write(sockfd, Name, strlen(Name) - 1);
					Name[strlen(Name)-1]=':';
					Name[strlen(Name)]=' ';
					strcpy(NameCpy,Name);
					continue;
				}

				strncat(Name,buffer,BUFFER_SIZE-1); /*Добавляем к имени текст сообщения*/

				write(sockfd, Name, strlen(Name) - 1);
				memset(&Name, 0, BUFFER_SIZE); /*Чистим буффер*/
				strcpy(Name,NameCpy); /*Восстанавливаем имя*/
	}
}
	return 0;
}

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
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	int result;
	fd_set readfds, testfds;
	char Name[NAME_SIZE];
	char Namebuf1[NAME_SIZE];
	char Namebuf2[NAME_SIZE];
	char Change[BUFFER_SIZE];

	server_sockfd = socket(AF_INET, SOCK_STREAM, 0); /*Создаём "слушающий" сокет*/

	server_address.sin_family = AF_INET; /*Протокол*/
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); /*IP текущего компьютера*/
	server_address.sin_port = htons(9734); /*Любой свободный порт*/

	server_len = sizeof(server_address);

	bind(server_sockfd, (struct sockaddr *)&server_address, server_len); /*Привязываем этот сокет*/    
	
	listen(server_sockfd, 5); /*Ставим его на "прослушку"*/

	FD_ZERO(&readfds); /*Очищаем множество сокетов для чтения*/
	FD_SET(server_sockfd, &readfds); /*Добавляем в него сервер*/

    while(1) /*Запускаем бесконечный цикл, в котором будем читать с сокетов-клиентов и отправлять им сообщения*/
	{
		char buffer[BUFFER_SIZE]; 
		int fd,max_fd,min_fd,i,j=0; /*i-счётчик, j-флаг состояния*/
		int nread;
		
		memset(&buffer, 0, BUFFER_SIZE); /*Чистим буффер*/

		testfds = readfds;
		printf("server waiting\n");
		
		result = select(FD_SETSIZE, &testfds, (fd_set *)0,(fd_set *)0, (struct timeval *)0); /*Ожидаем активность*/
		if (result < 1) 
		{
			perror("server5");
			exit(1);
		}

		for (fd = 0; fd < FD_SETSIZE; fd++) /*Проходим всю область индексных дискрипторов*/
		{
			if (FD_ISSET(fd, &testfds)) /*Если это наш дескриптор чтения*/
			{
				if (fd == server_sockfd) /*Если это дескриптор сервера*/
				{
					client_len = sizeof(client_address);
					client_sockfd = accept(server_sockfd,(struct sockaddr*)&client_address, &client_len); /*Принимаем запрос на подключение*/
					FD_SET(client_sockfd, &readfds);
					if(j==0) /*Определяем минимальный дискриптор (нужно для отправки сообщений клиентам)*/
					{
						min_fd=client_sockfd;
						j++;	
					}
					max_fd=client_sockfd; /*Определяем максимальный дискриптор*/
					printf("adding client on fd %d\n", client_sockfd);
					for(i=4;i<=max_fd;i++)
								if(i!=client_sockfd)
									write(i, "### New Client was conneted to server! ###", BUFFER_SIZE-1); /*ПРОБЛЕМА В ОТПРАВКЕ СООБЩЕНИЯ ВСЕМ*/
				}
				else 
				{
					ioctl(fd, FIONREAD, &nread); /*Получаем информацию о выводе*/
					if (nread == 0) /*Если клиент отключается от сервера*/
					{
						for(i=4;i<=max_fd;i++)
								if(i!=client_sockfd)
									write(i, "### Client leave the server! ###", BUFFER_SIZE-1); /*ПРОБЛЕМА В ОТПРАВКЕ СООБЩЕНИЯ ВСЕМ*/
						close(fd);
						FD_CLR(fd, &readfds); /*Удаляем дескриптор из множества на чтение*/
						printf("removing client on fd %d\n", fd);
					} 
					else 
					{
							memset(&buffer, 0, BUFFER_SIZE); /*Чистим буффер*/
							read(fd, buffer, BUFFER_SIZE-1); /*Если что-то можно прочитать - читаем*/
							printf("Message: ");
							printf("%s\n",buffer);
							if(!strcmp(buffer,"/nick")) /*Изменяем никнейм, заносим сообщение об изенении в буффер*/
							{
								printf("NICK!\n");
								memset(&Namebuf1, 0, BUFFER_SIZE); /*Чистим буффер*/
								memset(&Namebuf2, 0, BUFFER_SIZE); /*Чистим буффер*/
								read(fd, Namebuf1, BUFFER_SIZE-1);
								Namebuf1[strlen(Namebuf1)-1]=' ';
								read(fd, Namebuf2, BUFFER_SIZE-1);
								memset(&Change, 0, BUFFER_SIZE); /*Чистим буффер*/
								strcpy(Change,Namebuf1);
								strcat(Change,"now is known as ");
								strcat(Change,Namebuf2);
								strcpy(buffer,Change);
							}
							sleep(2);
							printf("serving client on fd %d\n", fd);
							for(i=4;i<=max_fd;i++) /*Отправляем сообщения всем*/
								if(i!=fd)
								{
									write(i, buffer, BUFFER_SIZE-1);
									printf("Writing to %d\n",i);
								}
							memset(&buffer, 0, BUFFER_SIZE); /*Чистим буффер*/
					}
		    	}
			}
		}
	}
return 0;
}

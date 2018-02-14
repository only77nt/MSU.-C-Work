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

#define BUFFER_SIZE 255
#define NAME_SIZE 30

int main() 
{
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	int result;
	fd_set readfds, testfds;
	char Name[NAME_SIZE];
	
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");;
	server_address.sin_port = htons(9734);

	server_len = sizeof(server_address);

	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);     
	
	listen(server_sockfd, 5);

	FD_ZERO(&readfds); /*Очищаем множество сокетов для чтения*/
	FD_SET(server_sockfd, &readfds); /*Добавляем в него сервер*/

    while(1) 
	{
		char buffer[BUFFER_SIZE]; 
		int fd;
		int nread;

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
					client_sockfd = accept(server_sockfd,(struct sockaddr*)&client_address, &client_len);
					FD_SET(client_sockfd, &readfds);
					printf("adding client on fd %d\n", client_sockfd);
				}
				else 
				{
					ioctl(fd, FIONREAD, &nread); /*Получаем информацию о выводе*/
					if (nread == 0) 
					{
						close(fd);
						FD_CLR(fd, &readfds); /*Удаляем дескриптор из множества на чтение*/
						printf("removing client on fd %d\n", fd);
					} 
					else 
					{
						read(fd, buffer, BUFFER_SIZE-1); /*Если что-то можно прочитать - читаем*/
						printf("Message: ");
						printf("%s\n",buffer);
						sleep(2);
						printf("serving client on fd %d\n", fd);
						write(fd, buffer, BUFFER_SIZE-1);
						memset(&buffer, 0, BUFFER_SIZE); /*Чистим буффер*/
					}
		    	}
			}
		}
	}
return 0;
}



#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include "libft.h"

typedef enum	e_cmd
{
	CONNECT, NICK, JOIN, WHO, MSG
}				t_cmd;

typedef struct	s_header
{
	t_cmd		cmd;
	char		**arg;
}				t_header;

int get_server_adress(char **av, char **server_ip, int *server_port)
{
	*server_ip = av[0];
	*server_port = ft_atoi(av[1]);
	return (EXIT_SUCCESS);
}

int main(int ac, char **av)
{
	int					sockfd;
	struct sockaddr_in	server;
	char				*line;
	char				server_reply[1000];
	char				*server_ip;
	int 				server_port;
	int 				size_read;

	if (ac != 3 || get_server_adress(++av, &server_ip, &server_port) == EXIT_FAILURE)
		return (1);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("\033[031msocket() error\033[0m\n");
		return (1);	
	}

// On rempli la structure sockaddr_in
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(server_ip);
	server.sin_port = htons(server_port);

// On se connecte au serveur
	if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		printf("\033[031mconnect() error\033[0m\n");
		return (1);
	}
	printf("\033[032mConnected to server\033[0m\n");

// On lit sur l'entrée standard et envoie le message au serveur
	printf("\n\033[033mEnter a msg:\033[0m\n");
	line = NULL;
	while ((size_read = get_next_line(0, &line)) > 0)
	{
		if (send(sockfd, line, strlen(line), 0) < 0)
		{
			printf("\033[031msend() error\033[0m\n");
			return (1);
		}
		ft_bzero(&line, size_read);
		ft_bzero(&server_reply, 1000);
		if (recv(sockfd, server_reply, 1000, 0) < 0)
		{
			printf("\033[031mrecv() error\033[0m\n");
			return (1);		
		}
		printf("\033[033mServer reply: \033[0m\n");
		write(1, server_reply, strlen(server_reply));
	}
	return (0);
}

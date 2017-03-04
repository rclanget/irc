#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include "libft.h"

int main(int ac, char **av)
{
	int 				sockfd;
	int 				sockfd_client;
	struct sockaddr_in 	server;
	struct sockaddr_in 	client;
	int 				c;
	char				buf_client[500];
	int 				read_size;
	struct protoent		*ip_protocol;
	int					port;

	bzero(buf_client, 500);
	if (ac > 2)
		return (EXIT_FAILURE);
	else if (ac == 2)
		port = ft_atoi(av[1]);
	else
		port = 6666;
// On récupère la valeur du protocol
	ip_protocol = getprotobyname("ip");
// On créé une socket
	sockfd = socket(AF_INET, SOCK_STREAM, ip_protocol->p_proto);
	if (sockfd == -1)
	{
		printf("\033[031msocket() error\033[0m\n");
		return (1);	
	}

// On rempli la structure sockaddr_in
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

// Lie la structure sockaddr au socket
	if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		printf("\033[031mbind() error\033[0m\n");
		return (1);
	}
	printf("\033[032mSocked created\033[0m\n");

// On écoute sur la socket
	listen(sockfd, 3);
	printf("\033[032mSocked listen, port:\033[0m %d\n", port);


// On attend qu'on client se connecte
	printf("\033[033mWait for connection...\033[0m\n");
	c = sizeof(struct sockaddr_in);
	sockfd_client = accept(sockfd, (struct sockaddr *)&client, (socklen_t *)&c);
	if (sockfd_client == 0)
	{
		printf("\033[031maccept() error\033[0m\n");
		return (1);
	}
	printf("\033[032mConnection accepted\033[0m\n");

// Réception des données envoyées par le client
	while ((read_size = recv(sockfd_client, buf_client, 500, 0)) > 0)
	{
		printf("\033[033mNew msg: \033[0m %s\n", buf_client);
		write(sockfd_client, "Message send\n", 13);
		bzero(buf_client, 500);
	}
	// Fermeture des socket
	close(sockfd);
	close(sockfd_client);

	if (read_size == 0)
	{
		printf("\033[031mClient disconnected\033[0m\n");
		return (0);
	}
	printf("\033[031mreceiv() error\033[0m\n");
	return (1);
}

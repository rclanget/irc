#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include "client.h"
#include "libft.h"

int ft_server_connection(t_client *client)
{
	struct sockaddr_in	server;
	if (client->server_ip)
	{
		ft_printf("type: %d\n", client->type_ip);
		server.sin_family = client->type_ip;
		server.sin_addr.s_addr = inet_addr(client->server_ip);
		server.sin_port = htons(client->server_port);
		if (connect(client->sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
		{
			ft_printf("\033[031mconnect() error\033[0m\n");
			return (EXIT_FAILURE);
		}
		client->connected = 1;
		ft_printf("\033[032mConnected to server\033[0m\n");
	}
	return (EXIT_SUCCESS);
}

int ft_get_server_adress(t_client *client, int ac, char **av)
{
	if (ac < 4)
	{
		if (ac == 1)
			return (EXIT_SUCCESS);
		client->server_ip = av[0];
		if (ac == 3)
			client->server_port = ft_atoi(av[1]);
		else
			client->server_port = 6697;
		client->type_ip = ft_strchr(client->server_ip, ':') ? AF_INET6 : AF_INET;
		return (EXIT_SUCCESS);
	}
	ft_printf("ft_usage: ./client [machine [port]]");
	return (EXIT_FAILURE);
}

int ft_create_socket(t_client *client)
{
	if ((client->sockfd = socket(client->type_ip, SOCK_STREAM, 0)) == -1)
	{
		ft_printf("\033[031msocket() error\033[0m\n");
		return (EXIT_FAILURE);	
	}
	return (EXIT_SUCCESS);
}

int ft_init_client(t_client *client, int ac, char **av)
{
	ft_bzero(client, sizeof(t_client));
	client->username = ft_strdup("coucou");
	client->nickname = ft_strdup(client->username);
	if (ft_get_server_adress(client, ac, ++av) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (client->server_ip && ft_create_socket(client) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int ft_iscmd(char *str)
{
	if (!ft_strcmp("connect", str) || !ft_strcmp("join", str))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int ft_tablen(char **tab)
{
	int i;

	i = 0;
	while (tab && tab[i])
		i++;
	return (i);
}

int ft_treat_command(t_client *client, t_header *header)
{
	if (header->cmd == CONNECT)
	{
		if (ft_get_server_adress(client, 3, header->arg) == EXIT_FAILURE ||
		!client->server_ip ||
		ft_create_socket(client) == EXIT_FAILURE ||
		ft_server_connection(client) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	else if (client->connected)
	{
		// Traiter les autres commandes;
		return (EXIT_SUCCESS);
	}
	ft_printf("Merci de te connecter\n");
	return (EXIT_FAILURE);
}

t_cmd ft_find_cmd(char *str)
{
	if (!ft_strcmp(str, "connect"))
		return (CONNECT);
	else if (!ft_strcmp(str, "nick"))
		return (NICK);
	else if (!ft_strcmp(str, "join"))
		return (JOIN);
	else if (!ft_strcmp(str, "who"))
		return (WHO);
	else
		return (MSG);
}

int ft_get_command(t_client *client, t_header *header, char *line)
{
	char	**tab;
	int		tab_len;

	tab = ft_strsplit(line, ' ');
	tab_len = ft_tablen(tab);
	if (!tab_len || ft_iscmd(tab[0]) == EXIT_FAILURE)
	{
		ft_printf("Commande inconnue\n");
		return (EXIT_FAILURE);
	}
	header->cmd = ft_find_cmd(tab[0]);
	header->arg = tab + 1;
	return (ft_treat_command(client, header));
}

int main(int ac, char **av)
{
	t_client			client;
	char				*line;
	char				server_reply[1000];
	int 				size_read;
	t_header 			header;

	if (ft_init_client(&client, ac, av) == EXIT_FAILURE ||
	ft_server_connection(&client) == EXIT_FAILURE)
		return (EXIT_FAILURE);

// On lit sur l'entrée standard et envoie le message au serveur
	ft_printf("\n\033[033mEnter a msg:\033[0m\n");
	line = NULL;
	while (get_next_line(0, &line) > 0)
	{
		size_read = ft_strlen(line);
		if (size_read && ft_get_command(&client, &header, line) == EXIT_SUCCESS)
		{
			ft_printf("ouaiche\n");
			if (send(client.sockfd, line, size_read, 0) < 0)
			{
				ft_printf("\033[031msend() error\033[0m\n");
				return (1);
			}
			ft_bzero(&line, size_read);
			if (recv(client.sockfd, server_reply, 1000, 0) < 0)
			{
				ft_printf("\033[031mrecv() error\033[0m\n");
				return (1);		
			}
			ft_printf("ouaiche 2\n");
			ft_printf("\033[033mServer reply: \033[0m\n");
			write(1, server_reply, strlen(server_reply));
		}
	}
	return (EXIT_SUCCESS);
}

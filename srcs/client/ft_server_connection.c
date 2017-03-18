#include "client.h"
#include "libft.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static int	ft_server_disconnect(t_client *client)
{
	close(client->sockfd);
	client->connected = 0;
	return (EXIT_SUCCESS);
}

int			ft_server_connection(t_client *client)
{
	struct sockaddr_in	server;
	t_header			header;

	if (client->connected)
	{
		ft_server_disconnect(client);
		if (ft_create_socket(client) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	if (client->server_ip)
	{
		server.sin_family = client->type_ip;
		server.sin_addr.s_addr = inet_addr(client->server_ip);
		server.sin_port = htons(client->server_port);
		if (connect(client->sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
		{
			ft_printf("\033[031mconnect() error\033[0m\n");
			return (EXIT_FAILURE);
		}
		client->connected = 1;
		header.cmd = NICK;
		ft_memcpy(header.from, client->username, 10);
		ft_server_send_request(client, &header);
	}
	return (EXIT_SUCCESS);
}
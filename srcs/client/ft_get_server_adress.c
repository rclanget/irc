#include "client.h"
#include "libft.h"
#include <netinet/in.h>

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
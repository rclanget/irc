#include "client.h"
#include "libft.h"
#include <sys/socket.h>

int ft_create_socket(t_client *client)
{
	if ((client->sockfd = socket(client->type_ip, SOCK_STREAM, 0)) == -1)
	{
		ft_printf("\033[031msocket() error\033[0m\n");
		return (EXIT_FAILURE);	
	}
	return (EXIT_SUCCESS);
}
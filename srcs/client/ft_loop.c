#include "client.h"
#include "libft.h"
#include <sys/select.h>

int ft_loop(t_client *client)
{
	struct timeval	tv;
	fd_set			rd;
	int				ret;

	ft_printf("\033[033m?> \033[0m");
	while (1)
	{
		tv.tv_sec = 120;
		tv.tv_usec = 0;
		FD_ZERO(&rd);
		FD_SET(0, &rd);
		if (client->connected)
			FD_SET(client->sockfd, &rd);
		ret = select(client->connected ? (client->sockfd + 1) : 1,  &rd,  NULL, NULL, &tv);
		if (ret == -1)
			return (EXIT_FAILURE);
		if (ret && FD_ISSET(0, &rd))
			ft_get_stdin(client);
		if (ret && FD_ISSET(client->sockfd, &rd))
			ft_handle_server_msg(client);
		ft_printf("\n\033[033m?> \033[0m");
	}
	return (EXIT_SUCCESS);
}
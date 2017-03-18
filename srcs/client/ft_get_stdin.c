#include "client.h"
#include "libft.h"

       #include <sys/types.h>
       #include <sys/socket.h>

#define NOT_CONNECTED -1

int ft_cmd_connect(t_client *client, t_header *header)
{
	char **tab;

	tab = ft_strsplit(header->msg, ' ');
	if (ft_tablen(tab) > 1)
	{
		if (ft_get_server_adress(client, ft_tablen(tab), tab + 1) == EXIT_SUCCESS &&
			ft_create_socket(client) == EXIT_SUCCESS &&
			ft_server_connection(client) == EXIT_SUCCESS)
		{
			// ft_freetab((void **)tab);
			return (EXIT_FAILURE);		
		}
		// ft_freetab((void **)tab);
	}
	return (EXIT_FAILURE);
}

int	ft_cmd_query(t_client *client, t_header *header)
{
	t_channel	*channel;
	t_channel	*new;
	t_channel	*old;
	char		**tab;

	channel = client->channel;
	new = NULL;
	old = NULL;
	tab = ft_strsplit(header->msg, ' ');
	if (ft_tablen(tab) > 1)
	{
		while (channel)
		{
			if (!ft_strcmp(channel->name, tab[1]))
				new = channel;
			if (channel->current)
				old = channel;
			channel = channel->next;
		}
		if (new)
		{
			new->current = 1;
			old->current = 0;
			ft_print_channel(client);
			// return (EXIT_SUCCESS);
		}
		// ft_freetab((void **)tab);
	}
	return (EXIT_FAILURE);
}

int	ft_server_send_request(t_client *client, t_header *header)
{
	if (header->cmd != CONNECT && !client->connected)
		return (NOT_CONNECTED);
	else if (header->cmd == CONNECT)
		return (ft_cmd_connect(client, header));
	else if (header->cmd == QUERY)
		return (ft_cmd_query(client, header));
	// else if (header->cmd == QUIT)
	// 	return (ft_cmd_quit(client, header));
	else
	{
		if (header->cmd == MSG)
		{
			ft_cmd_msg(client, header);
			ft_print_channel(client);
		}
		if (send(client->sockfd, header, sizeof(t_header), 0) < 0)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int ft_get_stdin(t_client *client)
{
	char		*line;
	t_header	header;
	int			ret;

	line = NULL;
	if (get_next_line(0, &line) == -1)
		return (EXIT_FAILURE);
	ret = ft_prepares_request(client, &header, line);
	if (ret == EXIT_FAILURE)
	{
		ft_printf("Command error");
		return (EXIT_FAILURE);
	}
	ret = ft_server_send_request(client, &header);
	if (ret != EXIT_SUCCESS)
	{
		if (ret == NOT_CONNECTED)
			ft_printf("Please connect first");
		else
			ft_printf("Server error");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
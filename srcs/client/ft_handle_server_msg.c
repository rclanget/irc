#include "client.h"
#include "libft.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>

int ft_cmd_join(t_client *client, t_header *header)
{
	t_channel *new;

	new = (t_channel *)malloc(sizeof(t_channel));
	new->current = 1;
	ft_memcpy(new->name, header->msg + 5, 20);
	new->fd = open(new->name, O_RDWR | S_IRWXU | O_CREAT | O_APPEND);
	if (new->fd < 0)
	{
		free(new);		
		return (EXIT_FAILURE);
	}
	new->prev = NULL;
	new->next = client->channel;
	if (new->next)
	{
		new->next->prev = new;
		new->next->current = 0;		
	}
	client->channel = new;
	ft_print_channel(client);
	return (EXIT_SUCCESS);
}

int ft_cmd_leave(t_client *client)
{
	t_channel *channel;
	t_channel *tmp_prev;
	t_channel *tmp_next;

	channel = client->channel;
	while (channel && !channel->current)
		channel = channel->next;
	if (channel)
	{
		tmp_prev = channel->prev;
		tmp_next = channel->next;
		if (tmp_prev)
			tmp_prev->next = tmp_prev;
		else
			client->channel = tmp_next;
		if (tmp_next)
			tmp_next->prev = tmp_prev;
		free(channel);
		if (client->channel)
			client->channel->current = 1;
	}
	return (EXIT_SUCCESS);
}

int ft_cmd_nick(t_client *client, t_header *header)
{
	ft_bzero(client->username, 10);
	ft_memcpy(client->username, header->from, 9);
	return (EXIT_SUCCESS);
}

int ft_apply_request(t_client *client)
{
	t_header header;

	header = ft_save_request(NULL);
	if (header.cmd == JOIN)
		return (ft_cmd_join(client, &header));
	else if (header.cmd == LEAVE)
		return (ft_cmd_leave(client));
	else if (header.cmd == NICK)
		return (ft_cmd_nick(client, &header));
	// else if (header.cmd == WHO)
	// 	return (ft_cmd_who(client, header));
	return (EXIT_FAILURE);
}

int ft_cmd_msg(t_client *client, t_header *header)
{
	t_channel 	*channel;

	if (!client->channel && ft_cmd_join(client, header) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	channel = client->channel;
	if (header->private_msg)
	{
		while (channel)
		{
			if (!ft_strcmp(channel->name, header->to))
			{
				lseek(channel->fd, 0, SEEK_END);
				ft_fdprint(channel->fd, "%s: %s\n", header->from, header->msg + 6);
				// write(channel->fd, header->msg + 6, ft_strlen(header->msg) - 6);
				// write(client->channel->fd, "\n", 1);
				return (EXIT_SUCCESS);
			}
			channel = channel->next;
		}
	}
	else
	{
		lseek(client->channel->fd, 0, SEEK_END);
		ft_fdprint(client->channel->fd, "%s: %s\n", header->from, header->msg);
		// write(client->channel->fd, header->msg, ft_strlen(header->msg));
		// write(client->channel->fd, "\n", 1);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

int ft_print_channel(t_client *client)
{
	t_channel	*channel;
	char		line[500];
	int			r;
	struct stat buf;

	channel = client->channel;
	while (channel && !channel->current)
		channel = channel->next;
	if (channel)
	{

		lseek(channel->fd, 0, SEEK_SET);
		fstat(channel->fd, &buf);
		ft_printf("\033c %s\n", channel->name);
		r = read(channel->fd, line, buf.st_size);
		line[r] = 0;
		ft_printf("%s", line);
	}
	return (EXIT_SUCCESS);
}

int ft_handle_server_msg(t_client *client)
{
	t_header header;

	recv(client->sockfd, &header, sizeof(t_header), 0);
	if (header.cmd == MSG)
	{
		if(ft_cmd_msg(client, &header) == EXIT_FAILURE)
		{
			ft_printf("Error recv msg");
			return (EXIT_FAILURE);			
		}
		ft_print_channel(client);
	}
	else if (header.cmd == RESPONSE && header.valid)
		return (ft_apply_request(client));
	return (EXIT_SUCCESS);
}
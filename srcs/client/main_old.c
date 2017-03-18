#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> // open

#include "client.h"
#include "libft.h"

int ft_iscmd(char *str)
{
	if (!ft_strcmp("/connect", str) ||
		!ft_strcmp("/join", str) ||
		!ft_strcmp("/nick", str) ||
		!ft_strcmp("/who", str) ||
		!ft_strcmp("/msg", str))
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

int ft_cmd_join(t_client *client, t_header *header)
{
	char **tab;

	tab = ft_strsplit(header->msg, ' ');
	if (ft_tablen(tab) > 1)
	{
		ft_memcpy(header->channel, tab[1], ft_strlen(tab[1]));
		ft_memcpy(header->from, client->nickname, 20);
		ft_memcpy(client->channel, tab[1], ft_strlen(tab[1]));
		return (EXIT_SUCCESS);
	}
	ft_print("Usage: /join <#channel>");
	return (EXIT_FAILURE);
}

int ft_cmd_msg(t_client *client, t_header *header)
{
	char **tab;

	tab = ft_strsplit(header->msg, ' ');
	if (ft_tablen(tab) > 2)
	{
		ft_memcpy(header->from, client->nickname, 20);
		ft_memcpy(header->to, tab[1], 20);
		header->private_msg = 1;
		return (EXIT_SUCCESS);
	}
	ft_print("Usage: /msg <to> <msg> ");
	return (EXIT_FAILURE);
}

int ft_cmd_msg_channel(t_client *client, t_header *header)
{
	char	msg[512];

	header->cmd = MSG;
	ft_bzero(msg, 512);
	ft_memcpy(msg, "/msg ", 5);
	ft_memcpy(msg + 5, client->channel, ft_strlen(client->channel));
	msg[ft_strlen(msg)] = ' ';
	ft_memcpy(msg + ft_strlen(msg), header->msg, 512 - ft_strlen(msg));
	ft_memcpy(header->msg, msg, 512);
	ft_memcpy(header->from, client->nickname, 20);
	ft_memcpy(header->channel, client->channel, 20);
	header->private_msg = 0;
	return (EXIT_SUCCESS);
}

int ft_cmd_nick(t_header *header)
{
	char **tab;

	tab = ft_strsplit(header->msg, ' ');
	if (ft_tablen(tab) > 1)
	{
		ft_memcpy(header->from, tab[1], 20);
		return (EXIT_SUCCESS);
	}
	ft_print("Usage: /nick <nickname>");
	return (EXIT_FAILURE);
}

int ft_treat_command(t_client *client, t_header *header)
{
	char	**arg;

	if (header->cmd == CONNECT)
	{
		if (!(arg = ft_strsplit(header->msg, ' ')))
			return (EXIT_FAILURE);
		if (ft_get_server_adress(client, ft_tablen(arg), arg + 1) == EXIT_FAILURE ||
		!client->server_ip ||
		ft_create_socket(client) == EXIT_FAILURE ||
		ft_server_connection(client) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		header->cmd = NICK;
		ft_memcpy(header->from, client->nickname, 20);
		return (EXIT_SUCCESS);
	}
	else if (client->connected)
	{
		if (header->cmd == JOIN)
			return (ft_cmd_join(client, header));
		else if (header->cmd == MSG)
			return (ft_cmd_msg(client, header));
		else if (header->cmd == MSG_CHANNEL)
			return (ft_cmd_msg_channel(client, header));
		else if (header->cmd == NICK)
			return (ft_cmd_nick(header));
		else if (header->cmd == WHO)
			return (EXIT_SUCCESS);
		return (EXIT_SUCCESS);
	}
	ft_printf("Merci de vous connecter");
	return (EXIT_FAILURE);
}

t_cmd ft_find_cmd(char *str)
{
	if (!ft_strcmp(str, "/connect"))
		return (CONNECT);
	else if (!ft_strcmp(str, "/nick"))
		return (NICK);
	else if (!ft_strcmp(str, "/join"))
		return (JOIN);
	else if (!ft_strcmp(str, "/who"))
		return (WHO);
	else if (!ft_strcmp(str, "/msg"))
		return (MSG);
	else
		return (MSG_CHANNEL);
}

int ft_get_command(t_client *client, t_header *header, char *line)
{
	char	**tab;
	int		tab_len;

	tab = ft_strsplit(line, ' ');
	tab_len = ft_tablen(tab);
	ft_bzero(header, sizeof(t_header));
	if (!tab_len || ft_iscmd(tab[0]) == EXIT_FAILURE)
	{
		if (!ft_strlen(client->channel))
		{
			ft_printf("Merci de vous connecter à une channel");
			return (EXIT_FAILURE);
		}
		else if (!client->connected)
		{
			ft_printf("Merci de vous connecter");
			return (EXIT_FAILURE);			
		}
		header->cmd = MSG_CHANNEL;
	}
	else
		header->cmd = ft_find_cmd(tab[0]);
	ft_memcpy(header->msg, line, 512);
	return (EXIT_SUCCESS);
}

int ft_send_command(t_client *client, t_header *header)
{
	if (send(client->sockfd, header, sizeof(t_header), 0) < 0)
	{
		ft_printf("\033[031msend() error\033[0m\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int  ft_read_file(char *name)
{
	char	file_name[25];
	char	*buf[500];
	int		fd;

	ft_printf("\033[033m%d\033[0m", __LINE__);
	file_name[0] = '.';
	ft_memcpy(file_name + 1, name, 20);
	ft_memcpy(file_name + ft_strlen(file_name), ".irc", 4);
	ft_printf("-> %s\n", file_name);
	if ((fd = open(file_name, O_RDONLY)) < 0)
		return (EXIT_FAILURE);
	// fd = open(file_name, O_RDONLY);
	ft_printf("\033[033m%d\033[0m", __LINE__);
	while (read(fd, buf, 1) > 0)
	{
		buf[1] = 0;		
		write(1, buf, 1);
	}
	close(fd);
	ft_printf("\033[033m%d\033[0m", __LINE__);
	return (EXIT_SUCCESS);
}

int ft_print_output(t_client *client, char *name)
{
	if (!ft_strcmp(client->channel, name))
		return (ft_read_file(name));
	return (EXIT_SUCCESS);
}

int ft_store_in_file(t_header *header, char *name)
{
	int fd;
	char	file_name[25];

	if (!name && !ft_strlen(name))
		return (EXIT_FAILURE);
	file_name[0] = '.';
	ft_memcpy(file_name + 1, name, 20);
	ft_memcpy(file_name + ft_strlen(file_name), ".irc", 4);
	if ((fd = open(file_name, O_RDWR | S_IRWXU | O_CREAT | O_APPEND)) < 0)
		return (EXIT_FAILURE);
	write(fd, header->msg, ft_strlen(header->msg));
	write(fd, "\n", 1);
	close(fd);
	return (EXIT_SUCCESS);
}

int ft_handle_server_msg(t_client *client, t_header *header)
{
	char **tab;

	ft_printf("cmd: [%d], msg: [%s]", header->msg, header->msg);
	if (recv(client->sockfd, header, sizeof(t_header), 0) < 0)
		ft_printf("\033[031mrecv() error\033[0m\n");
	if (header->cmd == MSG)
		return (ft_store_in_file(header, header->private_msg ? header->from : header->channel) && 
				ft_print_output(client, header->private_msg ? header->from : header->channel));
	else if (header->cmd == NICK)
	{
		tab = ft_strsplit(header->msg, ' ');
		ft_bzero(client->nickname, 20);
		ft_memcpy(client->nickname, tab[1], 20);
		ft_printf("new nickname: %s\n", client->nickname);
	}
	else
		ft_printf("Server: %s", header->msg);
	return (EXIT_SUCCESS);
}

int ft_loop(t_client *client)
{
	char		*line;
	t_header 	header;
	struct timeval tv;
	fd_set		rd;
	int 		ret;

	ft_printf("\033[033m?> \033[0m");
	line = NULL;
	while (1)
	{
		tv.tv_sec = 120;
		tv.tv_usec = 0;
		FD_ZERO(&rd);
		FD_SET(0, &rd);
		if (client->connected)
			FD_SET(client->sockfd, &rd);
		ret = select(client->connected ? (client->sockfd + 1) : 1,  &rd,  NULL, NULL, &tv);
		// ret = select(1,  &rd,  NULL, NULL, &tv);
		if (ret && FD_ISSET(0, &rd))
		{
			ft_bzero(&header, sizeof(t_header));
			get_next_line(0, &line);
			client->line = line;
			if (ft_get_command(client, &header, line) == EXIT_SUCCESS &&
			ft_treat_command(client, &header) == EXIT_SUCCESS)
			{
				if (ft_send_command(client, &header) == EXIT_FAILURE)
					return (EXIT_FAILURE);
			}
		}
		else if (ret && FD_ISSET(client->sockfd, &rd))
			ft_handle_server_msg(client, &header);
		ft_printf("\n\033[033m?> \033[0m");
	}
	return (EXIT_SUCCESS);
}

int main(int ac, char **av)
{
	t_client	client;

	if (ft_init_client(&client, ac, av) == EXIT_FAILURE ||
	ft_server_connection(&client) == EXIT_FAILURE ||
	ft_loop(&client) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

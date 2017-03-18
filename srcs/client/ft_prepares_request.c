#include "client.h"
#include "libft.h"

static int		ft_get_senderandrecipient(t_header *header, char *line)
{
	char		**tab;
	int			ret;

	ret = EXIT_SUCCESS;
	if ((header->cmd == MSG && header->private_msg) || header->cmd == NICK)
	{
		tab = ft_strsplit(line, ' ');
		if (ft_tablen(tab) > 1)
		{
			if (header->cmd == NICK)
				ft_memcpy(header->from, tab[1], 20);
			else
				ft_memcpy(header->to, tab[1], 20);				
		}
		else if (header->cmd == NICK)
			ret = EXIT_FAILURE;			
		ft_freetab((void **)tab);
	}
	return (ret);
}

static char		ft_is_private(t_cmd cmd, char *line)
{
	char		ret;

	ret = (cmd == MSG && line[0] == '/') ? 1 : 0;
	return (ret);
}

static t_cmd	ft_get_command(char *line)
{
	char		**tab;
	t_cmd		cmd;

	cmd = MSG;
	if (line && line[0] == '/')
	{
		tab = ft_strsplit(line, ' ');
		if (ft_tablen(tab))
		{
			if (!ft_strcmp(tab[0], "/connect"))
				cmd = CONNECT;
			else if (!ft_strcmp(tab[0], "/nick"))
				cmd = NICK;
			else if (!ft_strcmp(tab[0], "/join"))
				cmd = JOIN;
			else if (!ft_strcmp(tab[0], "/quit"))
				cmd = QUIT;
			else if (!ft_strcmp(tab[0], "/who"))
				cmd = WHO;
			else if (!ft_strcmp(tab[0], "/msg"))
				cmd = MSG;
			else if (!ft_strcmp(tab[0], "/leave"))
				cmd = LEAVE;
			else if (!ft_strcmp(tab[0], "/query"))
				cmd = QUERY;
			else
				cmd = -1;
		}
		else
			cmd = -1;
		ft_freetab((void **)tab);
	}
	return (cmd);
}

int				ft_prepares_request(t_client *client, t_header *header, char *line)
{
	ft_bzero(header, sizeof(t_header));
	if ((header->cmd = ft_get_command(line)) == NONE)
		return (EXIT_FAILURE);
	header->private_msg = ft_is_private(header->cmd, line);
	ft_memcpy(header->from, client->username, 10);
	ft_get_senderandrecipient(header, line);
	ft_memcpy(header->msg, line, 512);
	ft_save_request(header);
	return (EXIT_SUCCESS);
}
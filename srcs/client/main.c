#include "client.h"
#include "libft.h"

int ft_get_username(t_client *client)
{
	extern char **environ;
	char		**tab;

	(void)client;
	while (*environ)
	{
		if (ft_strstr(*environ, "USER"))
		{
			tab = ft_strsplit(*environ, '=');
			if (ft_tablen(tab) > 1)
			{
				ft_memcpy(client->username, tab[1], 9);
				ft_freetab((void **)tab);
				break ;
			}
		}
		++environ;
	}
	if (!*client->username)
		ft_memcpy(client->username, "user", 4);
	return (EXIT_SUCCESS);
}

int ft_init_client(t_client *client, int ac, char **av)
{
	ft_bzero(client, sizeof(t_client));
	if (ft_get_username(client) == EXIT_FAILURE ||
		ft_get_server_adress(client, ac, ++av) == EXIT_FAILURE ||
		(client->server_ip && ft_create_socket(client) == EXIT_FAILURE))
	{
		return (EXIT_FAILURE);		
	}
	return (EXIT_SUCCESS);
}

int main(int ac, char **av)
{
	t_client	client;

	if (ft_init_client(&client, ac, av) == EXIT_FAILURE ||
		ft_server_connection(&client) == EXIT_FAILURE ||
		ft_loop(&client) == EXIT_FAILURE)
	{
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
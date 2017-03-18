#include <stdlib.h>

int ft_freetab(void **tab)
{
	if (*tab)
	{
		ft_freetab(tab + 1);
		free(*tab);
	}
	return (EXIT_SUCCESS);
}

int ft_tablen(char **tab)
{
	int i;

	i = 0;
	while (tab && tab[i])
		++i;
	return (i);
}
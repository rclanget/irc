#include "client.h"
#include "libft.h"

t_header ft_save_request(t_header *header)
{
	static t_header save;

	if (header)
		ft_memcpy(&save, header, sizeof(t_header));
	return (save);
}
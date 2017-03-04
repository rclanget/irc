#ifndef CLIENT_H
# define CLIENT_H

typedef enum	e_cmd
{
	CONNECT, NICK, JOIN, WHO, MSG
}				t_cmd;

typedef struct	s_header
{
	t_cmd		cmd;
	char		**arg;
}				t_header;

typedef struct 	s_client
{
	char		*username;
	char		*nickname;
	char		*server_ip;
	short		type_ip;
	int			server_port;
	short		connected;
	int			sockfd;
}				t_client;

#endif
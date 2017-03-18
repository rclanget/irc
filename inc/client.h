#ifndef CLIENT_H
# define CLIENT_H

typedef struct			s_channel
{
	int					current;
	int					fd;
	char				name[20];
	struct s_channel	*prev;
	struct s_channel	*next;
}						t_channel;

typedef enum			e_cmd
{
	CONNECT, RESPONSE, NICK, JOIN, QUIT, WHO, MSG, LEAVE, QUERY, NONE
}						t_cmd;

typedef struct			s_header
{
	t_cmd				cmd;
	char        		valid;
	char        		private_msg;
	char        		channel[20];
	char        		from[20];
	char        		to[20];
	char        		msg[512];
}						t_header;

typedef struct 			s_client
{
	char				username[10];
	char				*server_ip;
	short				type_ip;
	int					server_port;
	short				connected;
	int					sockfd;
	t_channel			*channel;
	char				*line;
}						t_client;

/*
*	ft_create_socket.c
*/
int 					ft_create_socket(t_client *client);

/*
*	ft_server_connection.c
*/
int						ft_server_connection(t_client *client);

/*
*	ft_get_server_adress.c
*/
int						ft_get_server_adress(t_client *client, int ac, char **av);

/*
*	util.c
*/
int 					ft_freetab(void **tab);
int 					ft_tablen(char **tab);

/*
*	ft_loop.c
*/
int						ft_loop(t_client *client);

/*
*	ft_get_stdin.c
*/
int						ft_get_stdin(t_client *client);
int						ft_server_send_request(t_client *client, t_header *header);
/*
*	ft_handle_server_msg.c
*/
int						ft_handle_server_msg(t_client *client);

int ft_cmd_msg(t_client *client, t_header *header);
int ft_print_channel(t_client *client);

/*
*	ft_save_request.c
*/
t_header				ft_save_request(t_header *header);

/*
*	ft_prepares_request.c
*/
int						ft_prepares_request(t_client *client, t_header *header, char *line);

#endif
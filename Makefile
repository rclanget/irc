CLIENT		=	./client
SERVER		=	./server

CC			=	gcc
CFLAGS		=	-Werror -Wall -Wextra

# CLIENT
CLIENT_SRC_PATH	=	./srcs/client/
CLIENT_SRC_NAME	=	main.c \
					ft_create_socket.c \
					ft_server_connection.c \
					ft_get_server_adress.c \
					util.c \
					ft_loop.c \
					ft_get_stdin.c \
					ft_handle_server_msg.c \
					ft_save_request.c \
					ft_prepares_request.c
CLIENT_SRC			=	$(addprefix $(CLIENT_SRC_PATH),$(CLIENT_SRC_NAME))

CLIENT_OBJ_PATH	=	./obj/client/
CLIENT_OBJ_NAME	=	$(CLIENT_SRC_NAME:.c=.o)
CLIENT_OBJ			=	$(addprefix $(CLIENT_OBJ_PATH),$(CLIENT_OBJ_NAME))

# SERVER
SERVER_SRC_PATH	=	./srcs/server/
SERVER_SRC_NAME	=	main.c
SERVER_SRC			=	$(addprefix $(SERVER_SRC_PATH),$(SERVER_SRC_NAME))

SERVER_OBJ_PATH	=	./obj/server/
SERVER_OBJ_NAME	=	$(SERVER_SRC_NAME:.c=.o)
SERVER_OBJ			=	$(addprefix $(SERVER_OBJ_PATH),$(SERVER_OBJ_NAME))

INC_PATH	=	./inc/ ./libft/includes/
INC			=	$(addprefix -I,$(INC_PATH))

LIB			=	-L ./libft -lft


all: $(CLIENT) $(SERVER)

$(CLIENT): $(CLIENT_OBJ)
	@$(CC) $(LDFLAGS) $(CLIENT_OBJ) $(LIB) -o $@ && \
		printf " -->> \033[32mCompilation Success: %s\033[0m             \n" "$@"|| \
		printf " -->> \033[31mCompilation Failed: %s\033[0m              \n" "$@";
	@rm -rfv $(CLIENT_OBJ_PATH)

$(CLIENT_OBJ_PATH)%.o: $(CLIENT_SRC_PATH)%.c
	@make -C libft
	@mkdir -p $(CLIENT_OBJ_PATH)
	@$(CC) $(CFLAGS) -o $@ -c $< $(INC) && \
		printf " -->> \033[32mOk\033[0m: %s                       \r" "$@" || \
		printf " -->> \033[31mKo\033[0m: %s                       \r" "$@";

$(SERVER): $(SERVER_OBJ)
	@$(CC) $(LDFLAGS) $(SERVER_OBJ) $(LIB) -o $@ && \
		printf " -->> \033[32mCompilation Success: %s\033[0m             \n" "$@"|| \
		printf " -->> \033[31mCompilation Failed: %s\033[0m              \n" "$@";
	@rm -rfv $(SERVER_OBJ_PATH)

$(SERVER_OBJ_PATH)%.o: $(SERVER_SRC_PATH)%.c
	@make -C libft
	@mkdir -p $(SERVER_OBJ_PATH)
	@$(CC) $(CFLAGS) -o $@ -c $< $(INC) && \
		printf " -->> \033[32mOk\033[0m: %s                       \r" "$@" || \
		printf " -->> \033[31mKo\033[0m: %s                       \r" "$@";

client: $(CLIENT)

server: $(SERVER)

clean:
	@make clean -C libft
	@rm -rfv $(CLIENT_OBJ_PATH)
	@rm -rfv $(SERVER_OBJ_PATH)

fclean: clean
	@make fclean -C libft
	@rm -fv $(CLIENT)
	@rm -fv $(SERVER)
	@rm -rf ./obj

re: fclean all

norme:
	@norminette $(CLIENT_SRC)
	@norminette $(SERVER_SRC)
	@norminette $(INC_PATH)

.PHONY: all, clean, fclean, re, norme, client, server
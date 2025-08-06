#	PROGRAM NAME AND SOURCE FILES

NAME = minishell

SRCS = \
	exec.c \
	exec_cmd.c \
	exec_pipe.c \
	free_expr.c \
	free_token.c \
	lex.c \
	main.c \
	parse.c \
	parse_cmd.c \
	parse_pipe.c \
	print_expr.c \
	print_token.c \

#	CONFIG

CC = cc
CFLAGS += -Wall -Wextra -Werror -g3
OBJS_PATH = objs/

LIBFT_PATH = libft/
LIBFT_NAME = ft
LIBFT = $(LIBFT_PATH)lib$(LIBFT_NAME).a
IFLAGS += -I$(LIBFT_PATH)
LFLAGS += -L$(LIBFT_PATH) -l$(LIBFT_NAME)

#	VARS

OBJS = $(addprefix $(OBJS_PATH), ${SRCS:.c=.o})

#	RULES

all: $(NAME)

bonus: all

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LFLAGS)

$(OBJS_PATH)%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(IFLAGS) -c -o $@ $?

$(LIBFT):
	$(MAKE) -C $(LIBFT_PATH)

clean:
	rm -rf $(OBJS_PATH)
	$(MAKE) -C $(LIBFT_PATH) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_PATH) fclean

re: fclean all

.PHONY: all bonus clean fclean re

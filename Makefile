#	PROGRAM NAME AND SOURCE FILES

NAME = minishell

SRCS = \
	arg_list.c \
	builtin_cd.c \
	builtin_echo.c \
	builtin_env.c \
	builtin_exit.c \
	builtin_export.c \
	export_utils.c \
	builtin_pwd.c \
	builtin_unset.c \
	exec_expr.c \
	exec_binop.c \
	exec_builtin.c \
	exec_cmd.c \
	exec_info.c \
	exec_parentheses.c \
	exec_pipe.c \
	expand_arg.c \
	free_expr.c \
	free_token.c \
	handle_exit_status.c \
	is_builtin.c \
	lex.c \
	main.c \
	make_prompt.c \
	parse_cmd.c \
	parse.c \
	parse_expr.c \
	parse_binop.c \
	parse_parentheses.c \
	print_error.c \
	print_expr.c \
	print_token.c \
	redirection.c \
	resolve_exec_path.c \
	run_child.c \
	run_cmd.c \
	signals.c \
	termios.c \
	utils.c \
	wildcards.c \
	list_utils.c \
	wait_all.c \
	get_env.c \
	options.c \
	heredoc_utils.c \
	readline_utils.c \
	set_unset_env.c \
	token_utils.c \
	main_utils.c \
	var.c \
	var_utils.c \
	expand_utils.c \
	path_utils.c \
	fork_and_pipe.c

#	CONFIG

CC = cc
CFLAGS += -Wall -Wextra -Werror -g3
FSNTZ += -fsanitize=address
VLGRD += valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose
OBJS_PATH = objs/

LIBFT_PATH = libft/
LIBFT_NAME = ft
LIBFT = $(LIBFT_PATH)lib$(LIBFT_NAME).a
IFLAGS += -I$(LIBFT_PATH)
LFLAGS += -L$(LIBFT_PATH) -l$(LIBFT_NAME)

CFLAGS += -DUSE_READLINE=true
LFLAGS += -lreadline

#	VARS

OBJS = $(addprefix $(OBJS_PATH), ${SRCS:.c=.o})

#	RULES

all: $(NAME)

bonus: all

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LFLAGS)

d: debug | leaks

debug: $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(FSNTZ) -o $@ $(OBJS) $(LFLAGS)

leaks: $(NAME) $(LIBFT)
	$(VLGRD) ./$(NAME)

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
	rm -f debug
	rm -f leaks
	$(MAKE) -C $(LIBFT_PATH) fclean

re: fclean all

.PHONY: all bonus clean fclean re

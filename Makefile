#	PROGRAM NAME AND SOURCE FILES

NAME = minishell

SRCS = \
	main.c

#	CONFIG

CC = cc
CFLAGS += -Wall -Wextra -Werror -O3
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

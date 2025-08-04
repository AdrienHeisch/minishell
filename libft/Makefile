#	PROGRAM NAME AND SOURCE FILES

NAME = libft.a

SRCS = \
	ft_ctype0.c \
	ft_ctype1.c \
	ft_ft0.c \
	ft_ft1.c \
	ft_ft2.c \
	ft_ft3.c \
	ft_mem0.c \
	ft_mem1.c \
	ft_split.c \
	ft_std0.c \
	ft_str0.c \
	ft_str1.c \
	ft_str2.c \
	ft_string0.c \
	ft_string1.c \
	ft_string2.c \
	ft_lst0.c \
	ft_lst1.c \
	ft_lst2.c \
	ft_file0.c \
	ft_printf/ft_printf.c \
	ft_printf/ft_printf_char.c \
	ft_printf/ft_printf_hex.c \
	ft_printf/ft_printf_parsing.c \
	ft_printf/ft_printf_pointer.c \
	ft_printf/ft_printf_sint.c \
	ft_printf/ft_printf_string.c \
	ft_printf/ft_printf_uint.c \
	ft_printf/ft_printf_utils.c \
	get_next_line/get_next_line.c \
	get_next_line/get_next_line_utils.c \

#	CONFIG

CC = cc
CFLAGS += -Wall -Wextra -Werror
OBJS_PATH = objs/

#	VARS

OBJS = $(addprefix $(OBJS_PATH), ${SRCS:.c=.o})

#	RULES

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) -r $(NAME) $?

$(OBJS_PATH)%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(IFLAGS) -c $? -o $@

clean:
	rm -rf $(OBJS_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

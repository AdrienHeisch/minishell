NAME     =	pipex

# 	Compiler  #
CC       =	cc

#	Flags     #
CFLAGS   =	-Wall -Wextra -Werror -O3 -g3 #-fsanitize=address

# 	Headers   #
HEADER_P = 	includes
HEADER_N =	pipex.h
HEADER   =	$(addprefix $(HEADER_P)/,$(HEADER_N))

#	Sources   #
SRC =		command_assemble.c \
			commands.c \
			free.c \
			pipex_utils.c \
			pipex.c


DEP  = $(OBJ:%.o=%.d)

#	Objects   #
OBJ_PATH =	.objs
OBJ_NAME =	$(SRC:.c=.o)
OBJ      =	$(addprefix $(OBJ_PATH)/,$(OBJ_NAME))


################################################################################

all : $(NAME) 

-include $(DEP)

$(NAME) : $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $@

$(OBJ_PATH)/%.o : %.c
	@mkdir -p $(OBJ_PATH)
	@$(CC) $(CFLAGS) -I$(HEADER_P) -o $@ -c $<
	@printf "\e[1;30m$(CC): \e[1;37m./%-51s\e[1;0m" "$<"
	@printf "\e[32mcheck\e[1;0m\n"

save : fclean
	git add .
	git commit -m "$m"
	git push

clean :
	@rm -rf $(OBJ_PATH) 

fclean : clean
	@rm -rf $(NAME)

re : fclean all

.PHONY: all clean fclean re

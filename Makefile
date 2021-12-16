NAME = micro

all : $(NAME)

$(NAME) :
	@gcc -g -Wall -Wextra -Werror micro.c -o $(NAME)

clean :
	@rm -rf $(NAME)
	@rm -rf *.dSYM

fclean : clean

re : fclean all

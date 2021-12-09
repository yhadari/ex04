NAME = a.out

all : $(NAME)

$(NAME) :
	@gcc -Wall -Wextra -Werror micro.c -o $(NAME)

clean :
	@rm -rf $(NAME)
	@rm -rf *.dSYM

fclean : clean

re : fclean all

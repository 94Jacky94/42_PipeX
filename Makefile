NAME=pipex

OBJ=ft_split.o ft_strjoin.o\
	parsing.o transfert.o\
	get_next_line.o get_next_line_utils.o\
	pipex.o\

CPPFLAGS= -g3 -Wall -Wextra -Werror 

all : $(NAME)

$(NAME) : $(OBJ) 
	cc $(OBJ) -o pipex

clean:
	rm -f *.o

fclean: clean
	rm -f pipex
	rm -f infile
	rm -f outfile

re : fclean all

#############
## PATERNS ##
#############
#$@: Nom de la regle
#$<: Nom de la premiere dépendance
#$^: Nom de la toute les dependances
%.o: %.c
	cc $(CPPFLAGS) -c $< -o $@

.PHONY: all clean fclean re

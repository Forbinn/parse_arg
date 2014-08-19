##
## Makefile
##
## Made by vincent leroy
## Login   <leroy_v@epitech.eu>
##
## Started on  Mon Aug 18 14:08:46 2014 vincent leroy
## Last update Tue Aug 19 11:56:25 2014 vincent leroy
##

SRC			= main.c \
			  parse_arg.c \
			  parse_error.c \
			  parse_short_opt.c \
			  parse_long_opt.c \

CFLAGS		= -Wall -Wextra

LDFLAGS		=

NAME		= arg
NAME_DEBUG	= $(NAME).debug

OBJ			= $(SRC:.c=.o)
OBJ_DEBUG	= $(SRC:.c=.debug.o)

RM			= rm -f

CC			= gcc

MAKE		= make -C

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(NAME)

clean:
	$(RM) $(OBJ) $(OBJ_DEBUG) *.swp *~ *#

fclean: clean
	$(RM) $(NAME) $(NAME_DEBUG)

re: fclean all

debug: CFLAGS += -ggdb3
debug: $(OBJ_DEBUG)
	$(CC) $(OBJ_DEBUG) $(LDFLAGS) -o $(NAME_DEBUG)

%.debug.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean re debug

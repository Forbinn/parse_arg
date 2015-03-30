##
## Makefile
##
## Made by vincent leroy
## Login   <leroy_v@epitech.eu>
##
## Started on  Mon Aug 18 14:08:46 2014 vincent leroy
## Last update Mon Mar 30 20:20:21 2015 vincent leroy
##

LIBSRC		= parse_arg.c \
			  parse_error.c \
			  parse_short_opt.c \
			  parse_long_opt.c \

SRC			= main.c \
			  $(LIBSRC)

CFLAGS		= -Wall -Wextra

LDFLAGS		=

NAME		= arg
NAME_DEBUG	= $(NAME).debug

LIBNAME		= libparse.a

OBJ			= $(SRC:.c=.o)
OBJ_DEBUG	= $(SRC:.c=.debug.o)
LIBOBJ		= $(LIBSRC:.c=.o)

RM			= rm -f

CC			= gcc
AR			= ar

MAKE		= make -C

all: $(NAME)
static: $(LIBNAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(NAME)

$(LIBNAME): $(LIBOBJ)
	$(AR) rsc $(LIBNAME) $(LIBOBJ)

clean:
	$(RM) $(OBJ) $(OBJ_DEBUG) *.swp *~ *#

fclean: clean
	$(RM) $(NAME) $(NAME_DEBUG) $(LIBNAME)

re: fclean all

debug: CFLAGS += -ggdb3
debug: $(OBJ_DEBUG)
	$(CC) $(OBJ_DEBUG) $(LDFLAGS) -o $(NAME_DEBUG)

%.debug.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean fclean re debug

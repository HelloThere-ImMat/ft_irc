################
### MAKEFILE ###
################

NAME		=	ircserv

###############
### SOURCES ###
###############

SRCS_PATH = srcs/

SRCS += Server.cpp
SRCS += DataServ.cpp
SRCS += Client.cpp
SRCS += main.cpp

vpath %.cpp $(SRCS_PATH)

############
### OBJS ###
############

PATH_OBJS	+= objs
OBJS		+= $(patsubst %.cpp, $(PATH_OBJS)/%.o, $(SRCS))

####################
### DEPENDENCIES ###
####################

DEPS_PATH = dependencies/

DEPS += Server.hpp
DEPS += DataServ.hpp
DEPS += Client.hpp
DEPS += irc.hpp

vpath %.hpp $(DEPS_PATH)

###################
### COMPILATION ###
###################

CC			=	c++

CFLAGS		+=	-Wall
CFLAGS		+=	-Wextra
CFLAGS		+=	-Werror
CFLAGS		+=	-std=c++98

ifeq ($(debug), true)
	CFLAGS	+= -fsanitize=address,undefined -g3
endif

#############
### RULES ###
#############

all		: $(NAME)

$(NAME)	: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJS)	: $(PATH_OBJS)/%.o : %.cpp Makefile $(DEPS)
	mkdir -p $(PATH_OBJS)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(DEPS_PATH)

clean	:
	rm -rf $(PATH_OBJS)

fclean	:	clean
	rm -f $(NAME)

re		:	fclean all

.PHONY	: all clean fclean re

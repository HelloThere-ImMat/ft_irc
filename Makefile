################
### MAKEFILE ###
################

NAME		=	irc

###############
### SOURCES ###
###############

SRCS_PATH = srcs/

SRCS += server.cpp
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

DEPS += server.hpp

vpath %.hpp $(DEPS_PATH)

###################
### COMPILATION ###
###################

CC			=	c++

FLAGS		+=	-Wall
FLAGS		+=	-Wextra
FLAGS		+=	-Werror
FLAGS		+=	-std=c++98

#############
### RULES ###
#############

all		: $(NAME)

$(NAME)	: $(OBJS)
	$(CC) $(FLAGS) $^ -o $@

$(OBJS)	: $(PATH_OBJS)/%.o : %.cpp Makefile $(DEPS)
	mkdir -p $(PATH_OBJS)
	$(CC) $(FLAGS) -c $< -o $@ -I $(DEPS_PATH)

clean	:
	rm -rf $(PATH_OBJS)

fclean	:	clean
	rm -f $(NAME)

re		:	fclean all

.PHONY	: all clean fclean re

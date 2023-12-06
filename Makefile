#############
### SHELL ###
#############

SHELL		= /usr/bin/bash

################
### MAKEFILE ###
################

NAME		=	ircserv

###############
### SOURCES ###
###############

SRCS_PATH += srcs/
SRCS_PATH += srcs/ServerCmds

SRCS += Server.cpp
SRCS += DataServ.cpp
SRCS += Client.cpp
SRCS += Signal.cpp
SRCS += ServerCmds.cpp
SRCS += ClientManager.cpp
SRCS += main.cpp
SRCS += Channel.cpp
SRCS += SendCmd.cpp
SRCS += join.cpp

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
DEPS += Signal.hpp
DEPS += ClientManager.hpp
DEPS += irc.hpp
DEPS += Channel.hpp
DEPS += SendCmd.hpp

vpath %.hpp $(DEPS_PATH)

##############
### TESTER ###
##############

TESTER_FOLDER	= test/
TESTER			= $(TESTER_FOLDER)/tester.sh

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

#################
#### DISPLAY ####
#################

RED='\033[0;31m'
GREEN='\033[1;32m'
ORANGE='\033[0;33m'
YELLOW='\033[1;33m'
BLUE='\033[1;36m'
NC='\033[0m' # No Color

ifndef ECHO
T := $(words $(SRCS))
N := x
C = $(words $N)$(eval N := x $N)

ECHOC = echo -ne "\r\033[2K"
ECHO = $(ECHOC) $(ORANGE) "[`expr $C '*' 100 / $T`%]"
endif

#############
### RULES ###
#############

all		: $(NAME)

$(NAME)	: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
	$(ECHOC) $(GREEN) "--> $(NAME) COMPILED !"$(NC)"\n\n"

$(OBJS)	: $(PATH_OBJS)/%.o : %.cpp Makefile $(DEPS)
	$(ECHO) $(ORANGE) "Compiling $<"
	mkdir -p $(PATH_OBJS)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(DEPS_PATH)

test:
	$(MAKE) -s
	$(ECHOC) $(BLUE) "TESTER"$(NC)"\n\n"
	$(TESTER)

linter:
	$(ECHOC) $(BLUE) "\n""LINTER:\n\n"$(NC)
	bear -- make re 1>/dev/null
	pre-commit run --all-files

clean	:
	rm -rf $(PATH_OBJS)
	$(ECHOC) $(GREEN) "--> .o files deleted !"$(NC)"\n"

fclean	:	clean
	rm -f $(NAME)
	$(ECHOC) $(GREEN) "--> $(NAME) deleted !"$(NC)"\n"

re		:	fclean
	echo -e $(YELLOW) "\nRebuilding..." $(NC)
	$(MAKE) -s

.PHONY	: all clean fclean re linter test
.SILENT	:

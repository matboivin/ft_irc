NAME 				=	ircserv

SHELL				= 	/bin/sh
RM 					= 	/bin/rm

.SUFFIXES:
.SUFFIXES:				.cpp .hpp .o

# FILES

INC_FILES 			= 	ft_irc.hpp \
						UserInterface.hpp \
						Channel.hpp \
						Client.hpp \
						CLIParser.hpp \
						Config.hpp \
						Parser.hpp \
						Message.hpp \
						numeric_replies.hpp \
						Server.hpp \
						server_operations.hpp \
						server_operations.tpp

SRC_FILES 			= 	main.cpp \
						Channel.cpp \
						Client.cpp \
						CLIParser.cpp \
						Config.cpp \
						Parser.cpp \
						Message.cpp \
						numeric_replies.cpp \
						Server.cpp \
						server_operations.cpp \
						utils.cpp

# tests
SRC_FILES 			+= 	test_parsing.cpp

OBJ_FILES			= $(SRC_FILES:%.cpp=%.o)

# DIRS AND PATHS

INC_DIR				= 	include
SRC_DIR				= 	src
OBJ_DIR				= 	obj

SUB_DIRS			=	client \
						parsing \
						server \
						utils

SRC_SUBDIRS			:=	$(addprefix $(SRC_DIR)/, $(SUB_DIRS))

INC					:=	$(addprefix $(INC_DIR)/, $(INC_FILES))
OBJ					:= 	$(addprefix $(OBJ_DIR)/, $(OBJ_FILES))

VPATH = 			$(SRC_DIR) $(SRC_SUBDIRS)

# COMPIL AND FLAGS

CPPFLAGS			:= 	-I$(INC_DIR)
CXX					:= 	clang++
CXXFLAGS 			:=	-Wall -Wextra -Werror #-std=c++98 -pedantic A REMETTRE PLUS TARD APRES LE MERGE

# COLORS

COL_GREEN 			:= \033[0;32m
COL_GREEN_B 		:= \033[1;32m
COL_BLUE_B 			:= \033[1;34m
COL_WHITE_B 		:= \033[1;37m
COL_YELLOW			:= \033[1;33m
COL_RESET 			:= \033[0m

# STYLES

STYLE_BOLD			:= \e[1m
STYLE_ITALIC		:= \e[3m
STYLE_RESET			:= \e[0m

# RULES

all: $(NAME)

# OBJ DIR

$(OBJ_DIR):
					@mkdir -p $@

# COMPILING

$(OBJ_DIR)/%.o : 	%.cpp
					@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<
					@echo -e "$(COL_YELLOW)[compilation]$(COL_RESET)$(COL_WHITE_B) objects $(COL_RESET)$(STYLE_ITALIC)$@$(STYLE_RESET) created"

# LINKING

$(NAME):			$(OBJ_DIR) $(OBJ) $(INC)
					@$(CXX) $(OBJ) -o $@
					@echo -e "$(STYLE_BOLD)[link]$(STYLE_RESET) ...linking objects..."
					@echo -e "$(COL_GREEN_B)[success]$(COL_WHITE_B) $@$(COL_RESET) created in working directory"

# DEBUG

debug:				$(NAME)
					./$(NAME) 0.0.0.0 6667 test

debug_replies:
					./tests/replies_tester.sh

show:
					@echo -e "VPATH: $(VPATH)"

# CLEAN

clean:
					@$(RM) -rf $(OBJ_DIR)
					@echo -e "$(COL_BLUE_B)[clean]$(COL_WHITE_B) objects $(COL_RESET)removed"

fclean:				clean
					@$(RM) -f $(NAME)
					@echo -e "$(COL_BLUE_B)[fclean]$(COL_WHITE_B) $(NAME) $(COL_RESET)removed"

re: 				fclean all

.PHONY:				all debug clean fclean re

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ikayiban <ikayiban@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/15 09:27:48 by tclouet           #+#    #+#              #
#    Updated: 2026/03/02 16:13:55 by ikayiban         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =		ircserv

CXX =		c++

CXXFLAGS =	-Wall -Wextra -Werror -std=c++98 -g3

HEADERS =	Server.hpp Clients/Client.hpp Channels/Channel.hpp Utils/Utils.hpp

SOURCES =	main.cpp Server.cpp \
			DataHandler/DataHandler.cpp \
			Commands/AuthenCmdHandler.cpp Commands/JoinCommand.cpp \
			Commands/KickCommand.cpp Commands/InviteCommand.cpp \
			Commands/TopicCommand.cpp Commands/ModeCommand.cpp \
			Commands/Privmsg.cpp Commands/PartCommand.cpp Commands/QuitCommand.cpp \
			Clients/Client.cpp Channels/Channel.cpp \
			Utils/Logs.cpp Utils/ProgramHandler.cpp Utils/ProgramParsing.cpp \
			Utils/ServerResponseUtils.cpp Utils/ServerUtils.cpp Utils/ChannelUtils.cpp \
			Utils/Utils.cpp

OBJDIR =	objects

OBJECTS = 	$(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))

OBJDIRS = 	$(sort $(dir $(OBJECTS)))

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIRS):
	mkdir -p $@
	
$(OBJDIR)/%.o: %.cpp $(HEADERS) | $(OBJDIRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

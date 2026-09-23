NAME        = ircserv
BOT_NAME    = ircbot

CC          = c++
CFLAGS      = -Wall -Wextra -Werror -std=c++98 -Iinc -MMD -MP

# Fuentes del servidor (EXCLUYE src/Bot.cpp)
SRCS        = src/main.cpp \
              src/Server.cpp \
              src/Client.cpp \
              src/Channel.cpp \
              src/command_handler/exceptions.cpp \
              src/command_handler/ACommand_Handler.cpp \
              src/command_handler/AChannelCommandHandler.cpp \
              src/command_handler/Pass.cpp \
              src/command_handler/User.cpp \
              src/command_handler/Nick.cpp \
              src/command_handler/Cap.cpp \
              src/command_handler/Ping.cpp \
              src/command_handler/List.cpp \
              src/command_handler/Join.cpp \
              src/command_handler/Part.cpp \
              src/command_handler/Topic.cpp \
              src/command_handler/Mode.cpp \
              src/command_handler/Kick.cpp \
              src/command_handler/Invite.cpp \
              src/command_handler/Privmsg.cpp \
              src/command_handler/Quit.cpp \
              src/command_handler/Who.cpp \
              src/command_handler/Whois.cpp

# Fuentes del bot
BOT_SRCS    = src/Bot.cpp

OBJS        = $(SRCS:src/%.cpp=obj/%.o)
BOT_OBJS    = $(BOT_SRCS:src/%.cpp=obj/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

bonus: $(NAME) $(BOT_NAME)

$(BOT_NAME): $(BOT_OBJS)
	$(CC) $(CFLAGS) $(BOT_OBJS) -o $(BOT_NAME)

obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj

fclean: clean
	rm -f $(NAME) $(BOT_NAME)

re: fclean all

.PHONY: all bonus clean fclean re
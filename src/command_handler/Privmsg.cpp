#include "Command_handler.hpp"
#include "Channel.hpp"
#include "command_excepts.hpp"
#include "response.hpp"

PrivmsgCommandHandler::PrivmsgCommandHandler(Server &server, Client &client) : AChannelCommandHandler(server, client)
{
}

PrivmsgCommandHandler::~PrivmsgCommandHandler()
{
}

void PrivmsgCommandHandler::execute(const std::vector<std::string> &params)
{
    if (params.empty())
        throw InvalidParametersException(_client.getNickname(), "PRIVMSG");

    if (params.size() < 2)
    {
        
        throw CommandException(ERR_NOTEXTTOSEND(_client.getNickname()));
    }

    std::string target = params[0];
    std::string message = params[1];

    
    if (target[0] == '#')
    {
        Channel *channel = _server.getChannelByName(target);
        if (!channel)
            throw InvalidChannelException(_client.getNickname(), target);

        if (!channel->isMember(_client.getFd()))
            throw CannotSendToChannelException(_client.getNickname(), target);

        
        std::string fullMsg = MSG_PRIVMSG(_client.get_prefix(), target, message);
        channel->broadcast(fullMsg, _client.getFd());
    }
    else 
    {
        Client *targetClient = _server.searchNickname(target);
        if (!targetClient)
            throw NoSuchNickException(_client.getNickname(), target);

        
        std::string fullMsg = MSG_PRIVMSG(_client.get_prefix(), target, message);
        targetClient->write(fullMsg);
    }
}












#include "Command_handler.hpp"
#include "command_excepts.hpp"
#include "response.hpp"
#include <sstream>

AChannelCommandHandler::AChannelCommandHandler(Server &server, Client &client) : AbstractCommandHandler(server, client)
{
}

AChannelCommandHandler::~AChannelCommandHandler()
{
}

std::vector<std::string> AChannelCommandHandler::parseChannelList(
    const std::string &channelList)
{
    std::vector<std::string> channels;
    std::stringstream ss(channelList);
    std::string channel;

    while (std::getline(ss, channel, ','))
    {
        if (!channel.empty())
            channels.push_back(channel);
    }

    return channels;
}

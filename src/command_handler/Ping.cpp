











#include "Command_handler.hpp"
#include "command_excepts.hpp"

PingCommandHandler::PingCommandHandler(Server &server, Client &client) : AbstractCommandHandler(server, client)
{
}

PingCommandHandler::~PingCommandHandler()
{
}

void PingCommandHandler::execute(const std::vector<std::string> &params)
{
    if (params.size() != 1)
        throw InvalidParametersException(_client.getNickname(), "PING");

    if (!_client.getPassOk())
        throw NotRegisteredException(_client.getNickname());

    std::string pingMessage = params[0];
    if (pingMessage.empty())
        throw InvalidPingException(_client.getNickname());

    _client.write("PONG :" + pingMessage);
}

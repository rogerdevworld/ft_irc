











#include "Command_handler.hpp"
#include "response.hpp"

CapCommandHandler::CapCommandHandler(Server &server, Client &client) : AbstractCommandHandler(server, client)
{
}

CapCommandHandler::~CapCommandHandler()
{
}

void CapCommandHandler::execute(const std::vector<std::string> &params)
{
    if (params.size() < 1)
        throw InvalidParametersException(_client.getNickname(), "CAP");

    std::string subcommand = params[0];
    if (subcommand == "LS")
    {
        _client.write("CAP * LS :multi-prefix");
    }
    else if (subcommand == "REQ")
    {
        _client.write("CAP * ACK :multi-prefix");
    }
    else if (subcommand == "END")
    {
        
    }
    else
    {
        throw InvalidParametersException(_client.getNickname(), "CAP");
    }
}
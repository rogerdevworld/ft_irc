











#include "Command_handler.hpp"
#include "command_excepts.hpp"
#include "response.hpp"
#include <map>
#include <iostream>
#include <sstream>



AbstractCommandHandler::AbstractCommandHandler(
    Server &server,
    Client &client)
    : _server(server), _client(client)
{
}

AbstractCommandHandler::AbstractCommandHandler(
    const AbstractCommandHandler &other)
    : _server(other._server), _client(other._client)
{
}

AbstractCommandHandler::~AbstractCommandHandler()
{
}

AbstractCommandHandler &AbstractCommandHandler::operator=(
    const AbstractCommandHandler &other)
{
    if (this != &other)
    {
        _server = other._server;
        _client = other._client;
    }
    return *this;
}



static void handlePass(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    PassCommandHandler handler(server, client);
    handler.execute(params);
}

static void handleNick(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    NickCommandHandler handler(server, client);
    handler.execute(params);
}

static void handleUser(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    UserCommandHandler handler(server, client);
    handler.execute(params);
}

static void handleCap(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    CapCommandHandler handler(server, client);
    handler.execute(params);
}

static void handlePing(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    PingCommandHandler handler(server, client);
    handler.execute(params);
}

static void handleQuit(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    QuitCommandHandler handler(server, client);
    handler.execute(params);
}

static void handlePrivmsg(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    PrivmsgCommandHandler handler(server, client);
    handler.execute(params);
}

static void handleList(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    ListCommandHandler handler(server, client);
    handler.execute(params);
}

static void handleWho(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    WhoChannelCommandHandler handler(server, client);
    handler.execute(params);
}

static void handleJoin(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    JoinChannelCommandHandler handler(server, client);
    handler.execute(params);
}

static void handlePart(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    PartChannelCommandHandler handler(server, client);
    handler.execute(params);
}

static void handleKick(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    KickChannelCommandHandler handler(server, client);
    handler.execute(params);
}

static void handleTopic(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    TopicCommandHandler handler(server, client);
    handler.execute(params);
}

static void handleMode(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    ModeCommandHandler handler(server, client);
    handler.execute(params);
}

static void handleInvite(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    InviteCommandHandler handler(server, client);
    handler.execute(params);
}

static void handleWhois(
    Server &server,
    Client &client,
    const std::vector<std::string> &params)
{
    WhoisCommandHandler handler(server, client);
    handler.execute(params);
}

static std::map<std::string, HandlerExecutor> &getCommands()
{
    static std::map<std::string, HandlerExecutor> commands;

    if (commands.empty())
    {
        commands["PASS"] = &handlePass;
        commands["NICK"] = &handleNick;
        commands["USER"] = &handleUser;
        commands["PING"] = &handlePing;
        commands["CAP"] = &handleCap;
        commands["QUIT"] = &handleQuit;
        commands["PRIVMSG"] = &handlePrivmsg;
        commands["LIST"] = &handleList;
        commands["WHO"] = &handleWho;
        commands["JOIN"] = &handleJoin;
        commands["PART"] = &handlePart;
        commands["KICK"] = &handleKick;
        commands["TOPIC"] = &handleTopic;
        commands["MODE"] = &handleMode;
        commands["INVITE"] = &handleInvite;
        commands["WHOIS"] = &handleWhois;
    }

    return (commands);
}

Command AbstractCommandHandler::parseLine(const std::string &line)
{
    Command command;
    std::istringstream iss(line); 

    if (!(iss >> command.name)) 
        return command;
    
    std::string rest;
    std::getline(iss, rest); 

    size_t start = rest.find_first_not_of(' '); 
    if (start == std::string::npos) 
        return command;

    rest = rest.substr(start);

    size_t pos = 0;
    while (pos < rest.size())
    {
        if (rest[pos] == ':') 
        {
            command.params.push_back(rest.substr(pos + 1));
            break;
        }

        size_t spacePos = rest.find(' ', pos); 
        if (spacePos == std::string::npos)
        {
            command.params.push_back(rest.substr(pos)); 
            break;
        }

        command.params.push_back(rest.substr(pos, spacePos - pos)); 
        pos = spacePos + 1;

        while (pos < rest.size() && rest[pos] == ' ') 
            pos++;
    }

    return command;
}

void AbstractCommandHandler::executeCommand(
    Server &server,
    Client &client,
    const std::string commandName,
    const std::vector<std::string> &params)
{
    std::map<std::string, HandlerExecutor> &commands = getCommands();

    std::map<std::string, HandlerExecutor>::iterator it =
        commands.find(commandName);

    if (it == commands.end())
    {
        throw CommandNotFoundException(
            client.getNickname(),
            commandName
        );
    }

    bool allowedBeforeRegister =
        (commandName == "PASS" || commandName == "NICK" ||
         commandName == "USER" || commandName == "CAP"  ||
         commandName == "PING" || commandName == "QUIT");

    if (!client.getRegistered() && !allowedBeforeRegister)
        throw NotRegisteredException(client.getNickname());

    it->second(server, client, params);
}

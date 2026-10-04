#include "Bot.hpp"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <ctime>
#include <csignal>

Bot::Bot(const std::string &host, int port, const std::string &password)
    : _host(host), _port(port), _password(password), _nickname("IRCBot"), _socketFd(-1) {}

Bot::~Bot() {
    if (_socketFd != -1)
        close(_socketFd);
}

void Bot::connectToServer() {
    _socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketFd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_port);
    inet_pton(AF_INET, _host.c_str(), &serverAddr.sin_addr);

    if (connect(_socketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        exit(1);
    }
}

void Bot::authenticate() {
    std::string passCmd = "PASS " + _password + "\r\n";
    std::string nickCmd = "NICK " + _nickname + "\r\n";
    std::string userCmd = "USER bot 0 * :IRC Assistant Bot\r\n";

    send(_socketFd, passCmd.c_str(), passCmd.size(), 0);
    send(_socketFd, nickCmd.c_str(), nickCmd.size(), 0);
    send(_socketFd, userCmd.c_str(), userCmd.size(), 0);
}

void Bot::handleCommand(const std::string &sender, const std::string &target, const std::string &message) {
    std::string replyTarget = (target[0] == '#') ? target : sender;
    std::string response;

    if (message.find("!time") == 0) {
        time_t now = time(NULL);
        std::string timeStr = ctime(&now);
        if (!timeStr.empty() && timeStr[timeStr.size() - 1] == '\n')
            timeStr.erase(timeStr.size() - 1);
        response = "PRIVMSG " + replyTarget + " :Current server time is: " + timeStr + "\r\n";
    } else if (message.find("!ping") == 0) {
        response = "PRIVMSG " + replyTarget + " :PONG!\r\n";
    } else if (message.find("!help") == 0) {
        response = "PRIVMSG " + replyTarget + " :Available commands: !time, !ping, !help\r\n";
    }

    if (!response.empty()) {
        send(_socketFd, response.c_str(), response.size(), 0);
    }
}

void Bot::listenAndRespond() {
    char buffer[1024];
    std::string incomingData;

    while (true) {
        int bytesReceived = recv(_socketFd, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) break;

        buffer[bytesReceived] = '\0';
        incomingData.append(buffer, bytesReceived);

        size_t pos;
        while ((pos = incomingData.find('\n')) != std::string::npos) {
            std::string line = incomingData.substr(0, pos);
            incomingData.erase(0, pos + 1);

            if (!line.empty() && line[line.size() - 1] == '\r')
                line.erase(line.size() - 1);

            if (line.empty())
                continue;

            // Responder PING del servidor
            size_t pingPos = line.find("PING");
            if (pingPos != std::string::npos) {
                std::string pingParam = line.substr(pingPos + 4);
                size_t start = pingParam.find_first_not_of(" ");
                if (start != std::string::npos) {
                    pingParam = pingParam.substr(start);
                    std::string pong = "PONG " + pingParam + "\r\n";
                    send(_socketFd, pong.c_str(), pong.size(), 0);
                }
                continue;
            }

            // Responder a INVITE
            if (line.find(" INVITE ") != std::string::npos) {
                size_t invitePos = line.find(" INVITE ");
                std::string rest = line.substr(invitePos + 8);
                size_t start = rest.find_first_not_of(" ");
                if (start != std::string::npos) {
                    rest = rest.substr(start);
                    size_t spacePos = rest.find(' ');
                    if (spacePos != std::string::npos) {
                        std::string channel = rest.substr(spacePos + 1);
                        if (!channel.empty() && channel[0] == ':')
                            channel = channel.substr(1);
                        std::string joinCmd = "JOIN " + channel + "\r\n";
                        send(_socketFd, joinCmd.c_str(), joinCmd.size(), 0);
                    }
                }
            }

            // Parser para PRIVMSG
            size_t privmsgPos = line.find(" PRIVMSG ");
            if (privmsgPos != std::string::npos) {
                std::string sender = "user";
                if (line[0] == ':') {
                    size_t exMark = line.find('!');
                    if (exMark != std::string::npos && exMark < privmsgPos)
                        sender = line.substr(1, exMark - 1);
                }

                std::string rest = line.substr(privmsgPos + 9);
                size_t spacePos = rest.find(' ');
                if (spacePos != std::string::npos) {
                    std::string target = rest.substr(0, spacePos);
                    size_t colonPos = rest.find(':', spacePos);
                    if (colonPos != std::string::npos) {
                        std::string msg = rest.substr(colonPos + 1);
                        handleCommand(sender, target, msg);
                    }
                }
            }
        }
    }
}

void Bot::run() {
    connectToServer();
    authenticate();
    listenAndRespond();
}

int main(int argc, char **argv) {
    signal(SIGPIPE, SIG_IGN);
    if (argc != 4) {
        std::cerr << "Usage: ./ircbot <host> <port> <password>" << std::endl;
        return 1;
    }
    Bot bot(argv[1], std::atoi(argv[2]), argv[3]);
    bot.run();
    return 0;
}
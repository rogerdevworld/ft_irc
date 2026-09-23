#include "Bot.hpp"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sstream>

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
        response = "PRIVMSG " + replyTarget + " :Current server time is: " + std::string(ctime(&now));
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
        incomingData += buffer;

        size_t pos;
        while ((pos = incomingData.find("\r\n")) != std::string::npos) {
            std::string line = incomingData.substr(0, pos);
            incomingData.erase(0, pos + 2);

            // Responder a PINGs del servidor para mantener la conexión
            if (line.find("PING") == 0) {
                std::string pong = "PONG " + line.substr(5) + "\r\n";
                send(_socketFd, pong.c_str(), pong.size(), 0);
                continue;
            }

            // Parsear PRIVMSG
            if (line.find("PRIVMSG") != std::string::npos) {
                size_t nickEnd = line.find('!');
                if (nickEnd != std::string::npos && line[0] == ':') {
                    std::string sender = line.substr(1, nickEnd - 1);
                    size_t msgStart = line.find("PRIVMSG ");
                    if (msgStart != std::string::npos) {
                        std::string rest = line.substr(msgStart + 8);
                        size_t targetEnd = rest.find(" ");
                        std::string target = rest.substr(0, targetEnd);
                        size_t textStart = rest.find(" :");
                        if (textStart != std::string::npos) {
                            std::string msg = rest.substr(textStart + 2);
                            handleCommand(sender, target, msg);
                        }
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
    if (argc != 4) {
        std::cerr << "Usage: ./ircbot <host> <port> <password>" << std::endl;
        return 1;
    }
    Bot bot(argv[1], std::atoi(argv[2]), argv[3]);
    bot.run();
    return 0;
}
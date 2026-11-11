#ifndef BOT_HPP
#define BOT_HPP

#include <string>
#include <cstdio>
#include <stdio.h>

class Bot {
private:
    std::string _host;
    int         _port;
    std::string _password;
    std::string _nickname;
    int         _socketFd;

    void connectToServer();
    void authenticate();
    void listenAndRespond();
    void handleCommand(const std::string &sender, const std::string &target, const std::string &message);

public:
    Bot(const std::string &host, int port, const std::string &password);
    ~Bot();
    void run();
};

#endif
#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <chrono>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/utsname.h>
#include <memory>
#include <list>
#include <unistd.h>
#include <iostream>
#include <sstream>

#define PING_COMMAND "PING\r\n"
#define PONG_RESPONSE "PONG"
#define BUFFER_SIZE 2048

using ms = std::chrono::duration<int, std::milli>;
typedef std::chrono::time_point<std::chrono::system_clock> timeType;

class Connection
{
public:
    Connection(char *ip, int port);
    ~Connection();

    bool is_alive(){ return is_alive_; };
    void recv_commands();
    std::string getCommand();
    void update_alive_status();
    void sendData(char *msg, size_t msg_size);
    void closeConnection();

private:
    int socket_;
    bool is_alive_;
    bool is_pinging_;
    timeType timePing_;
    std::list<std::string> commands_;

    void parse_commands(std::string s);
    void ping();
    void pongRecived();
};

#endif
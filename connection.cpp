#include "connection.h"

Connection::Connection(char *ip, int port)
{
    int status;
    struct sockaddr_in serv_addr;

    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ < 0)
    {
        is_alive_ = false;
        return;
    }
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
    status = inet_pton(AF_INET, ip, &serv_addr.sin_addr);
    if (status < 0)
    {
        is_alive_ = false;
        return;
    }
    std::cout << "Start connection. \n";
    status = connect(socket_, (struct sockaddr*)&serv_addr,
        sizeof(serv_addr));
	if (status < 0) {
		printf("Connection Failed \n");
		return;
	}
    std::cout << "Connection created. \n";
    timePing_ = std::chrono::system_clock::now();
    is_alive_ = true;
    is_pinging_ = false;
}

Connection::~Connection()
{
    closeConnection();
}

void Connection::closeConnection()
{
    if (!is_alive_)
        return;

    is_alive_ = false;
    std::cout << "Close connection. \n";
    close(socket_);
}

void Connection::sendData(char *msg, size_t msg_size)
{
    send(socket_, msg, msg_size, 0);
}

void Connection::pongRecived()
{
    is_pinging_ = false;
}

void Connection::ping()
{
    sendData((char *)PING_COMMAND, 6);
    is_pinging_ = true;
}

void Connection::parse_commands(std::string s)
{
    /* SPLIT and ADD to list */
    std::size_t temp;
    std::string cmd;

    while (s.size())
    {
        temp = s.find("\r\n");
        cmd = s.substr(0,  temp);
        if (cmd == "PONG")
            pongRecived();
        commands_.push_back( cmd );
        s = s.substr(temp + 2);
    }
}

std::string Connection::getCommand()
{
    std::string command;
    if (commands_.empty())
        return (command);
    command = commands_.back();
    commands_.pop_back();
    return (command);
}

void Connection::recv_commands()
{
    int status, valread;
    char buff[BUFFER_SIZE];
    fd_set rfds;
    struct timeval tv;
    std::stringstream temp;

    tv.tv_sec = 10;
    FD_ZERO(&rfds);
	FD_SET(socket_, &rfds);
    status = select(socket_ + 1, &rfds, NULL, NULL, &tv);
    switch (status)
    {
    case (0):
        /* TIMEOUT READ */
        if (is_pinging_)
        {
            std::cout << "Timeout PONG recieve. \n";
            closeConnection();
        }
        break;
    case (-1):
        closeConnection();
        break;
    default:
        do
        {
            valread = read(socket_, buff, BUFFER_SIZE - 1);
            switch (valread)
            {
            case (-1):
                closeConnection();
                return;
            default:
                buff[valread] = 0;
                temp << buff;
                break;
            }
        } while (valread == BUFFER_SIZE - 1);
        parse_commands(temp.str());
        break;
    }
}

void Connection::update_alive_status()
{
    int timeDelta;
    timeType tmpTime;

    tmpTime = std::chrono::system_clock::now();
    timeDelta = std::chrono::duration_cast<ms>(tmpTime - timePing_).count();
    if (is_pinging_)
    {
        if (timeDelta > 10000)
        {
            std::cout << PONG_RESPONSE << " not recieved. \n";
            closeConnection();
        }
    }
    else
    {
        if (timeDelta > 5000)
        {
            ping();
            timePing_ = std::chrono::system_clock::now();
        }
    }
}
#ifndef MAIN_HPP
#define MAIN_HPP

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <chrono>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/utsname.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define CLOSE_COMMAND "CLOSE_SESSION"
#define DATA_COMMAND "GET_DATA"
#define PING_MSG "PING"

typedef std::chrono::time_point<std::chrono::system_clock> timeType;


#endif
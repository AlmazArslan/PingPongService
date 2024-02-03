
#include "main.hpp"

using ms = std::chrono::duration<int, std::milli>;

int main(int argc, char const* argv[])
{
	int status, valread, sock, PORT;
	struct timeval tv;
	struct sockaddr_in serv_addr;
	struct utsname system_info;
	std::string sys_info_s;
	fd_set rfds;
	timeType lastPing;
	char buffer[BUFFER_SIZE] = { 0 };

	if (argc < 3)
	{
		printf("Need IP and PORT(example: ./PingPong 127.0.0.1 1234) \n");
		return -1;
	}

	uname(&system_info);
	sys_info_s.append(system_info.sysname);
	sys_info_s.append("\n");
	sys_info_s.append(system_info.nodename);
	sys_info_s.append("\n");
	sys_info_s.append(system_info.release);
	sys_info_s.append("\n");
	sys_info_s.append(system_info.version);
	sys_info_s.append("\n");
	sys_info_s.append(system_info.machine);

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		printf("Socket creation error \n");
		return -1;
	}

	PORT = atoi(argv[2]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	status = inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);
	if (status <= 0) {
		printf("Invalid address/ Address not supported \n");
		return -1;
	}

	status = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (status < 0) {
		printf("Connection Failed \n");
		return -1;
	}

	lastPing = std::chrono::system_clock::now();

	printf("Start main_loop \n");
	while (strcmp(buffer, CLOSE_COMMAND) != 0)
	{
		if (std::chrono::duration_cast<ms>(
			std::chrono::system_clock::now() - lastPing).count() > 1000)
		{
			printf("Send PING \n");
			send(sock, PING_MSG, strlen(PING_MSG), 0);
			tv.tv_sec = 10;
			FD_ZERO(&rfds);
			FD_SET(sock, &rfds);
			status = select(sock + 1, &rfds, NULL, NULL, &tv);
			switch (status)
			{
				case (0):
					printf("Timeout PONG \n");
					close(sock);
					return -1;
					break;
				case (-1):
					printf("Error \n");
					return -1;
					break;
				default:
					lastPing = std::chrono::system_clock::now();
			}

		}

		tv.tv_sec = 1;
		FD_ZERO(&rfds);
		FD_SET(sock, &rfds);
		status = select(sock + 1, &rfds, NULL, NULL, &tv);
		switch (status)
		{
		case (0):
			break;
		case (-1):
			printf("Error \n");
			break;
		default:
			valread = read(sock, buffer, BUFFER_SIZE - 1);
			buffer[valread] = 0;
			if (valread > 0)
			{
				printf("data = %s \n", buffer);
				if (strcmp(DATA_COMMAND, buffer) == 0)
				{
					printf("Get data command \n");
					send(sock, sys_info_s.c_str(), sys_info_s.length(), 0);
				}
			}
			break;
		}
	}
	printf("Close command received \n");
	close(sock);

	return 0;
}

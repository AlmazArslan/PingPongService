
#include "main.hpp"


int main(int argc, char const* argv[])
{
	int status, valread, sock, PORT, i;
	struct timeval tv;
	struct sockaddr_in serv_addr;
	struct utsname system_info;
	fd_set rfds;
	timeType lastPing;
	char buffer[BUFFER_SIZE] = { 0 };

	lastPing = std::chrono::system_clock::now();
	printf("%")
	if (argc < 3)
	{
		printf("Need IP and PORT(example: ./PingPong 127.0.0.1 1234) \n");
		return -4;
	}

	uname(&system_info);

	printf("%s\n", system_info.version);

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		printf("Socket creation error \n");
		return -3;
	}

	PORT = atoi(argv[2]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	status = inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);
	if (status <= 0) {
		printf("Invalid address/ Address not supported \n");
		return -2;
	}

	status = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (status < 0) {
		printf("Connection Failed \n");
		return -1;
	}

	FD_ZERO(&rfds);
	FD_SET(sock, &rfds);
	tv.tv_sec = 1;
	lastPing = std::chrono::system_clock::now();

	while (strcmp(buffer, CLOSE_COMMAND) != 0)
	{
		send(sock, PING_MSG, strlen(PING_MSG), 0);

		// printf("Send PONG \n");
		for (i = 0; i < 10; i++)
		{
			status = select(sock + 1, &rfds, NULL, NULL, &tv);
			if (status < 0)
			{
				printf("Error \n");
				return -5;
			}
			else
				break;
		}
		if (i == 10)
		{
			printf("Timeout PONG, close session");
			return 0;
		}
		valread = read(sock, buffer, BUFFER_SIZE - 1);
		if (strcmp("PONG", buffer) != 0 && valread > 0)
			printf("%s\n", buffer);
	}
	printf("Close command received");
	close(sock);

	return 0;
}

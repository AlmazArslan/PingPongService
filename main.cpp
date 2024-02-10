
#include "main.hpp"


int main(int argc, char *argv[])
{
	std::string tmpCommand;

	if (argc != 3)
		std::cout << "Need IP and PORT(example: ./PingPong 127.0.0.1 1234) \n";

	Connection conn(argv[1], atoi(argv[2]));
	CommandCenter cmdCenter;

	
	cmdCenter.addCommand(std::string("GET_DATA"),
		[](Connection &conn){
			struct utsname system_info;
			std::string sys_info_s;

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

			conn.sendData((char *)sys_info_s.c_str(), sys_info_s.size());
		});

	cmdCenter.addCommand(std::string("CLOSE_CONNECTION"),
		[](Connection &conn){
			conn.sendData((char *)"CLOSE_CONNECTION\r\n", 17);
			conn.closeConnection();
		});

	while (conn.is_alive())
	{
		conn.update_alive_status();
		conn.recv_commands();
		tmpCommand = conn.getCommand();
		if (!tmpCommand.empty())
		{
			std::cout << "CMD RECIEVED: " << tmpCommand << " \n";
			cmdCenter.handle(tmpCommand, conn);
		}
	}

	return 0;
}

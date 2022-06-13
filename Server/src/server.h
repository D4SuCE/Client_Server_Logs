#pragma once
#include <ctime>
#include <string>
#include <fstream>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")

#define SIZE 1024

namespace Net
{
	class Server
	{
	private:
		WSAData wsa;
		SOCKET serversocket;
		std::string ipaddress;
		int port;
		char buffer[SIZE];
		struct sockaddr_in info;
		int infolength;
		int recvlength;
		bool work;
	private:
		void init();
		void receive();
		void proccess();
	private:
		const std::string currentDateTime();
	public:
		Server(std::string, int);
		~Server();
	public:
		void start();
	};
}
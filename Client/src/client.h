#pragma once
#include <string>
#include <cassert>
#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")

namespace Net
{
	class Client
	{
	private:
		WSAData wsa;
		SOCKET clientsocket;
		std::string ipaddress;
		int port;
		std::string name;
		std::string message;
		struct sockaddr_in info;
		int infolength;
		int recvlength;
		bool work;
	private:
		void init();
		void setName();
		void send();
	private:
		void help();
	public:
		Client(std::string, int);
		~Client();
	public:
		void connect();
	};
}
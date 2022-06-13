#include "server.h"

#pragma warning(disable: 4996)

namespace Net
{
	Server::Server(std::string ipaddress, int port)
		:
		wsa{ 0 },
		ipaddress(ipaddress),
		port(port),
		serversocket(INVALID_SOCKET),
		info{ 0 },
		infolength(sizeof(info)),
		work(true) {};

	void Server::init()
	{
		info.sin_family = AF_INET;
		info.sin_port = htons(port);
		info.sin_addr.s_addr = inet_addr(ipaddress.c_str());
		
		std::cout << "WSA init" << std::endl;
		assert(!(WSAStartup(514, &wsa)) && "Couldn`t init WSA");
		std::cout << "WSA success!" << std::endl;

		std::cout << "Creating socket" << std::endl;
		assert(!((serversocket = socket(AF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR) && "Couldn`t create socket");
		std::cout << "Socket creating success!" << std::endl;

		std::cout << "Bind socket" << std::endl;
		assert(!(bind(serversocket, (struct sockaddr*)& info, infolength)) && "Couldn`t bind socket");
		std::cout << "Socket binded" << std::endl;

		std::cout << "Server started at: " << inet_ntoa(info.sin_addr) << ":" << ntohs(info.sin_port) << std::endl;
	}

	void Server::start()
	{
		init();
		while (work)
		{
			receive();
			proccess();
		}
	}

	void Server::receive()
	{
		if ((recvlength = recvfrom(serversocket, buffer, SIZE, 0, (struct sockaddr*)&info, &infolength)) == SOCKET_ERROR)
		{
			std::cout << "recvfrom() failed..." << WSAGetLastError() << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	void Server::proccess()
	{
		std::string message = "";
		for (unsigned i = 0; i < recvlength; i++)
		{
			message += buffer[i];
		}
		std::string name = message.substr(0, message.find(" "));
		std::string article = message.substr(message.find(" ") + 1);
		if (article.size() != 0 && article != " ")
		{
			std::fstream file;
			file.open("log.txt", std::fstream::out | std::fstream::app);
			file << "[" << currentDateTime() << "] " << name << ": " << article << std::endl;
			file.close();
		}

		int token = -1;

		if (article == "TERMINATE()")
		{
			token = 1000;
		}

		switch (token)
		{
			case 1000:
				work = false;
				break;
		}
	}

	const std::string Server::currentDateTime()
	{
		time_t now = time(0);
		struct tm tstruct = *localtime(&now);
		char buf[80];
		strftime(buf, sizeof(buf), "%d-%m-%Y|%X", &tstruct);
		return buf;
	}

	Server::~Server()
	{
		std::fstream file;
		file.open("log.txt", std::fstream::out | std::fstream::app);
		file << "[" << currentDateTime() << "] " << "SERVER" << ": " << "TERMINATED!" << std::endl;
		file.close();
		WSACleanup();
		closesocket(serversocket);
	}
}
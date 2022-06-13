#include "client.h"

#pragma warning(disable: 4996)

namespace Net
{
	Client::Client(std::string ipaddress, int port)
		:
		wsa{ 0 },
		ipaddress(ipaddress),
		port(port),
		name(name),
		clientsocket(INVALID_SOCKET),
		info{ 0 },
		infolength(sizeof(info)),
		work(true) {};

	void Client::init()
	{
		info.sin_family = AF_INET;
		info.sin_port = htons(port);
		info.sin_addr.s_addr = inet_addr(ipaddress.c_str());

		std::cout << "WSA init" << std::endl;
		assert(!(WSAStartup(514, &wsa)) && "Couldn`t init WSA");
		std::cout << "WSA success!" << std::endl;

		std::cout << "Creating socket" << std::endl;
		assert(!((clientsocket = socket(AF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR) && "Couldn`t create socket");
		std::cout << "Socket creating success!" << std::endl;		
	}

	void Client::setName()
	{
		std::cout << "Enter a name: ";
		std::cin >> name;
		std::getchar();
	}

	void Client::connect()
	{
		init();
		setName();
		message = name + " CONNECTED!";
		if ((sendto(clientsocket, message.c_str(), message.size(), 0, (struct sockaddr*)&info, infolength)) == SOCKET_ERROR)
		{
			std::cout << "sendto() failed..." << WSAGetLastError() << std::endl;
			exit(EXIT_FAILURE);
		}
		while (work)
		{
			send();
		}
	}

	void Client::send()
	{
		std::cout << "Enter a message: ";
		std::getline(std::cin, message);

		int token = -1;

		if (message == "HELP()")
		{
			token = 0;
		}
		else if (message == "EXIT()")
		{
			token = 99;
		}
		else if (message == "TERMINATE()")
		{
			token = 1000;
		}

		message.insert(0, name + " ");

		if ((sendto(clientsocket, message.c_str(), message.size(), 0, (struct sockaddr*)&info, infolength)) == SOCKET_ERROR)
		{
			std::cout << "sendto() failed..." << WSAGetLastError() << std::endl;
			exit(EXIT_FAILURE);
		}

		switch (token)
		{
			case 0:
				help();
				break;
			case 99:
				work = false;
				break;
			case 1000:
				work = false;
				break;
		}
	}

	void Client::help()
	{
		std::cout << "-------------------------------------" << std::endl;
		std::cout << "HELP() - information about commands." << std::endl;
		std::cout << "EXIT() - disconnect." << std::endl;
		std::cout << "TERMINATE() - shutdown server." << std::endl;
		std::cout << "-------------------------------------" << std::endl;
	}

	Client::~Client()
	{
		message = name + " DISCONNECTED!";
		if ((sendto(clientsocket, message.c_str(), message.size(), 0, (struct sockaddr*)&info, infolength)) == SOCKET_ERROR)
		{
			std::cout << "sendto() failed..." << WSAGetLastError() << std::endl;
			exit(EXIT_FAILURE);
		}
		WSACleanup();
		closesocket(clientsocket);
	}
}
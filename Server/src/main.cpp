#include "server.h"

int main()
{
	Net::Server server("127.0.0.1", 9999);
	server.start();
	return 0;
}
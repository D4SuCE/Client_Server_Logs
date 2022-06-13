#include "client.h"

int main()
{	
	Net::Client client("3.134.39.220", 443);
	client.connect();
	return 0;
}
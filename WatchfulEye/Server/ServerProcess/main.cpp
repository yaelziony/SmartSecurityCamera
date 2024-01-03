#include "Server.h"

int main() {
	Server& server = Server::getInstance();
	server.runServer();
	return 0;
}
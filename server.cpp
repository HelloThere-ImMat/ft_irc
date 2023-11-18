#include "server.hpp"

#define PORT 8080

int setUpServer(Data *obj)
{
	int opt = 1;

	//Setting address to local host
	obj->address.sin_family = AF_INET;
	obj->address.sin_addr.s_addr = INADDR_ANY;
	obj->address.sin_port = htons(PORT);

	try {
		if ((obj->servfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			throw("creating socket");
		if (setsockopt(obj->servfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
			throw("setting sock options");
		if (bind(obj->servfd, (struct sockaddr*)&obj->address, sizeof(obj->address)) < 0)
			throw("binding to address");
	}
	catch(std::string &e)
	{
		std::cout << "Error : Setup : " << e << std::endl;
		return (1);
	}
	return (0);
}

int main(int argc, char const* argv[])
{
	Data			data;
	char buffer[1024] = { 0 };
	std::string str = "Hello from server";
	const char* hello = str.c_str();
	(void)argc;
	(void)argv;

	if (setUpServer(&data) == 1)
		return (1);

	if (listen(data.servfd, 3) < 0) {
		std::cerr << "Error : while listening" << std::endl;
		return (1);
	}
	while (1)
	{
		if ((data.sockfd
			= accept(data.servfd, (struct sockaddr*)&data.address,
					&data.addrlen))
			< 0) {
			std::cerr << "Error : while accepting" << std::endl;
			return (1);
		}
		read(data.sockfd, buffer, 1024 - 1);	// subtract 1 for the null
												// terminator at the end
		std::cout << "Message received : " << buffer << std::endl;
		send(data.sockfd, hello, strlen(hello), 0);
		std::cout << "Hello message sent" << std::endl;
	}
	close(data.sockfd);
	close(data.servfd);
	return 0;
}

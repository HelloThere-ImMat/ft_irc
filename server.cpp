#include "server.hpp"

int	setUpServer(Data *const obj)
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
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int main(int ac, char const **av)
{
	char	buffer[1024] = { 0 };
	Data	data;

	if (ac == EXPECTED_ARG_COUNT)
	{
		std::cout << "Port is " << av[1] << std::endl;
		std::cout << "Password is " << av[2] << std::endl;
	}
	if (setUpServer(&data) == 1)
		return (EXIT_FAILURE);
	if (listen(data.servfd, MAX_CLIENT_COUNT) < 0) {
		std::cerr << "Error : while listening" << std::endl;
		return (EXIT_FAILURE);
	}
	while (true)
	{
		if ((data.sockfd
			= accept(data.servfd, (struct sockaddr*)&data.address,
					&data.addrlen)) < 0) {
			std::cerr << "Error : while accepting" << std::endl;
			return (EXIT_FAILURE);
		}
		read(data.sockfd, buffer, 1024 - 1);	// subtract 1 for the null
												// terminator at the end
		std::cout << "Message received : " << buffer << std::endl;
		send(data.sockfd, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE), 0);
		std::cout << "Hello message sent" << std::endl;
	}
	close(data.sockfd);
	close(data.servfd);
	return (EXIT_SUCCESS);
}

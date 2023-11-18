#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <iostream>


// int getaddrinfo(const char *node,     // e.g. "www.example.com" or IP
//                 const char *service,  // e.g. "http" or port number
//                 const struct addrinfo *hints,
//                 struct addrinfo **res);

int main()
{
	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo;  // will point to the results

	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_INET;     // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

	if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0) {
		printf("getaddrinfo error: %s\n", gai_strerror(status));
		exit(1);
	}

	std::cout << servinfo->ai_family << std::endl;
	std::cout << servinfo-> << std::endl;
	std::cout << servinfo->ai_family << std::endl;
	std::cout << servinfo->ai_family << std::endl;

}

#include "socket.h"

#define PORT 12345


/**
 * initialize_address - Initializes a 'sockaddr_in' structure for IPv4 addresses
 * This function sets the family to AF_INET (IPv4), the address to INADDR_ANY
 * (accepting connections on any IP), and the port to a predefined PORT number,
 * converting it from host byte order to network byte order. The 'sockaddr_in'
 * structure will be used for the server's address settings.
 * @address: A pointer to the 'sockaddr_in' structure to be initialized
 * Return: Nothing (void)
 * Author: Frank Onyema Orji
 */

void initialize_address(struct sockaddr_in *address)
{
	address->sin_family = AF_INET;
	address->sin_addr.s_addr = INADDR_ANY;
	address->sin_port = htons(PORT);
}

/**
 * create_and_setup_server_socket - Creates and sets up a server socket
 * This function handles the creation of a socket, setting socket options,
 * binding it to a specified address, and setting it to listen for connections.
 * @address: A pointer to a 'sockaddr_in' structure that contains the address
 * settings (port, IP) the server will use.
 * Return: The file descriptor for the created server socket;
 *         the program exits if any step fails (socket creation, setting options,
 *         binding, or listening).
 */

int create_and_setup_server_socket(struct sockaddr_in *address)
{
	int server_fd, opt = 1;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	if (bind(server_fd, (struct sockaddr *)address, sizeof(*address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	return (server_fd);
}
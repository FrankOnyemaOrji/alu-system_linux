#include "socket.h"

#define PORT 12345


/**
 * create_socket - Establishes a new TCP socket for IPv4 communication
 *
 * This function attempts to initialize a socket using the AF_INET address family
 * and SOCK_STREAM socket type. It's designed for TCP/IP networking.
 *
 * Return: On success, returns the newly created socket file descriptor.
 *         On failure, prints an error message to stderr and exits the program.
 *
 * Note: The caller is responsible for closing the socket when it's no longer needed.
 * Author: Frank Onyema Orji
 */

int create_socket(void)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
	{
		perror("Error creating socket");
		exit(EXIT_FAILURE);
	}
	return (sockfd);
}

 /**
 * setup_address - Configures server address settings
 *
 * This function populates a sockaddr_in structure with the necessary
 * information for IPv4 network communication. It sets:
 * - Address family to IPv4
 * - IP address to INADDR_ANY (accept connections on any interface)
 * - Port number to a predefined PORT value (converted to network byte order)
 *
 * @address: Pointer to the sockaddr_in structure to be configured
 *
 * Note: The PORT macro should be defined elsewhere in the code.
 *       This function assumes little-endian architecture for htons() usage.
 */

void setup_address(struct sockaddr_in *address)
{
	memset(address, 0, sizeof(*address));
	address->sin_family = AF_INET;
	address->sin_addr.s_addr = INADDR_ANY;
	address->sin_port = htons(PORT);
}

/**
 * bind_socket - program that binds the server socket to the specified
 * IP address and port number contained within the 'sockaddr_in' structure
 * if the socket cannot be bound (the address is in use), the function prints
 * an error message and exits the program
 * @sockfd: the socket file descriptor returned by create_socket()
 * @address: a pointer to a 'sockaddr_in' structure that contains the
 *           IP address and port number to which the socket will be bound
 * Return: nothing (void)
 */

void bind_socket(int sockfd, struct sockaddr_in *address)
{
	if (bind(sockfd, (struct sockaddr *)address, sizeof(*address)) < 0)
	{
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}
}

/**
 * start_listening - program that places the server socket in a listening
 * state, where it can accept connection requests
 * this function allows the socket to queue up to 5 incoming connection
 * requests before refusing additional requests;
 * if an error occurs while attempting to listen on the socket, an error
 * message is printed and the program terminates
 * @sockfd: the socket file descriptor on which the server will listen
 *          for incoming connections
 * Return: nothing (void)
 */

void start_listening(int sockfd)
{
	if (listen(sockfd, 5) < 0)
	{
		perror("Listen failed");
		exit(EXIT_FAILURE);
	}
	printf("Server listening on port %d\n", PORT);
}

/**
 * main - the entry point of the program
 * This function orchestrates the creation of a socket, sets up the server's
 * address, binds the socket, and starts listening for incoming connections
 * on port 12345. After setup, it enters an infinite loop to simulate a server
 * that runs indefinitely until terminated by an external signal.
 * Return: Always returns 0, although the program is designed to run
 * indefinitely and only terminates upon receiving a 'kill' signal.
 */

int main(void)
{
	struct sockaddr_in address;

	int sockfd = create_socket();

	setup_address(&address);
	bind_socket(sockfd, &address);
	start_listening(sockfd);

	/* Hang indefinitely until killed by a signal */
	while (1)
	{
		sleep(1);
	}

	return (0);
}

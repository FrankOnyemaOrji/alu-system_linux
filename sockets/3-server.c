#include "socket.h"

#define PORT 12345
#define BUFFER_SIZE 1024


/**
 * initialize_server - Initializes the server by creating a TCP socket,
 * binding it to a specific port, and setting it to listen for incoming connections.
 * This function configures the server's address to accept connections
 * on any network interface. If socket creation, binding, or listening fails,
 * it prints an error message and exits the program.
 * @server_fd: A pointer to an integer where the file descriptor of the
 *             created socket will be stored. This descriptor is used for all
 *             subsequent operations on the socket.
 * Return: Nothing (void)
 */

void initialize_server(int *server_fd)
{
	struct sockaddr_in address;

	*server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (*server_fd == -1)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(*server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(*server_fd, 1) < 0)
	{
		perror("listen failed");
		exit(EXIT_FAILURE);
	}
	printf("Server listening on port %d\n", PORT);
}

/**
 * accept_connection - Accepts an incoming connection request on the listening socket.
 * This function retrieves and prints the IP address of the connecting client.
 * @server_fd: The file descriptor of the server socket that is listening
 *             for connections.
 * Return: The file descriptor of the newly accepted client socket;
 *         if accepting the client fails, the function prints an error
 *         message and exits the program.
 */

int accept_connection(int server_fd)
{
	struct sockaddr_in client_address;
	socklen_t client_address_len = sizeof(client_address);
	char client_ip[INET_ADDRSTRLEN];
	int new_socket;

	new_socket = accept(server_fd, (struct sockaddr *)&client_address,
		&client_address_len);

	if (new_socket < 0)
	{
		perror("accept failed");
		exit(EXIT_FAILURE);
	}

	inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
	printf("Client connected: %s\n", client_ip);
	return (new_socket);
}

/**
 * handle_client - Handles communication with a connected client.
 *                 Receives a message, prints it, and closes the connection.
 * @client_socket: The file descriptor of the client socket.
 *
 * This function is responsible for:
 * 1. Receiving a message from the connected client.
 * 2. Printing the received message.
 * 3. Closing the client socket after the message is printed.
 * 
 * If receiving the message fails, it prints an error message and exits the program.
 * 
 * Return: void
 */

void handle_client(int client_socket)
{
	char buffer[BUFFER_SIZE];
	ssize_t message_len = recv(client_socket, buffer, BUFFER_SIZE, 0);

	if (message_len < 0)
	{
		perror("recv failed");
		exit(EXIT_FAILURE);
	}

	buffer[message_len] = '\0';
	printf("Message received: \"%s\"\n", buffer);
	close(client_socket);
}

/**
 * main - Entry point of the server program.
 *
 * This program initializes the server, accepts a connection from a client,
 * handles the client's request, and performs cleanup before exiting.
 *
 * The sequence of operations is as follows:
 * 1. Calls 'initialize_server' to set up the listening socket.
 * 2. Accepts a connection from a client using 'accept_connection'.
 * 3. Handles the client's message with 'handle_client'.
 * 4. Closes the server socket.
 *
 * Return: 0 on success.
 */

int main(void)
{
	int server_fd;
	int client_socket;

	initialize_server(&server_fd);

	client_socket = accept_connection(server_fd);

	handle_client(client_socket);

	close(server_fd);

	return (0);
}
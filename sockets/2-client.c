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
 * accept_connection - Accepts an incoming connection request on the listening socket
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

	hints_init(&hints);
	status = getaddrinfo(argv[1], argv[2], &hints, &host_addrinfo);
	if (status)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return (EXIT_FAILURE);
	}

	client_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client_id == -1)
	{
		perror("socket");
		return (EXIT_FAILURE);
	}

	for (tmp = host_addrinfo; tmp; tmp = tmp->ai_next)
	{
		if (connect(client_id, tmp->ai_addr, tmp->ai_addrlen) == 0)
		{
			printf("Connected to %s:%s\n", argv[1], argv[2]);
			freeaddrinfo(host_addrinfo);
			close(client_id);
			return (EXIT_SUCCESS);
		}
	}

	fprintf(stderr, "No valid address found for %s:%s\n", argv[1], argv[2]);
	freeaddrinfo(host_addrinfo);
	close(client_id);
	return (EXIT_FAILURE);
}
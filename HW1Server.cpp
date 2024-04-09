```cpp
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <vector>


void* client_handler(void* data)
{
	int client_socket = *(static_cast<int*>(data));
	char buffer[3001];
	// Receiving message from client
	while(true)
	{
		int received = recv(client_socket, buffer, 3000, 0);
        if (received < 0)
	    {
	       	std::perror("Error receiving data from client");
			close(client_socket);
            return nullptr;
		}
		else if (received > 0)
		{
			std::cout << "Received message:\n";
			buffer[received] = '\0';
			std::cout << buffer << "\n";
		}
		else
		{
			std::cout << "Client disconnected \n";
			break;
		}
	}
	close(client_socket);
	return nullptr;
}

int main()
{
	// Creating a socket
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_socket == -1)
	{
		std::perror("Error creating socket");
		exit(errno);
	}

	// Configuring socket address
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(9888);

	// Binding server_socket to server_address
	if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) 
	{
		std::perror("Failed to bind socket");
		exit(errno);
	}

	if (listen(server_socket, 1024) < 0) 
	{
		std::perror("Failed to listen on socket");
		exit(errno);
       	}
	std::cout << "Waiting for connections\n";
	
	std::vector<pthread_t> threads;

	while(true)
       	{
		int client_socket;
	        struct sockaddr_in client_address;
		unsigned int client_addr_len = sizeof(client_address);
		// Accepting incoming connection
		if ((client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_addr_len)) < 0) 
		{
			std::perror("Failed to accept connection");
		        exit(errno);
		}
		std::cout << "Client connected with address: " << inet_ntoa(client_address.sin_addr) << "\n";

		pthread_t thread;
		if(pthread_create(&thread, nullptr, client_handler, &client_socket) != 0)
		{
			std::perror("Failed to create thread");
			exit(EXIT_FAILURE);
		}
		threads.push_back(thread);
	}
	for(auto& x : threads)
	{
		if(pthread_join(x, nullptr) != 0)
		{
			std::perror("Failed to join thread");
			exit(EXIT_FAILURE);
		}
	}

	// Closing server socket
	close(server_socket);
	return 0;
}
```
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>

int main() 
{
	  // Configuration for server connection
	  struct sockaddr_in server_address;
	
	  // socket
	  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
	  if (client_socket == -1) 
	  {
		std::perror("Error creating socket");
	        exit(errno);
	  }
	  
	  // Configuring server IP and port
	  server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	  server_address.sin_family = AF_INET;
	  server_address.sin_port = htons(9888);
	  
	  // Connecting to the server
	  int connected = connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));
	  
	  if(connected == -1)
	  {
		std::perror("Failed to establish connection");
	  	exit(errno);
	  }
	  
	  // Sending messages to the server
	  std::string message;
	  while(message != "exit")
	  {
	  	std::cin >> message;
	  	int sent = send(client_socket, message.c_str(), message.size(), 0);
	  	if(sent == -1)
	  	{
			std::perror("Failed to send message");
	  		exit(errno);
	  	}
	  }
	  close(client_socket);
	  return 0;
}

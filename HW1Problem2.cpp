#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

const std::string SERVER_HOST = "www.ysu.am";
const std::string SERVER_PORT = "443";
const std::string REQUEST = "GET / HTTP/1.1\r\nHost: " + SERVER_HOST + "\r\nConnection: close\r\n\r\n";

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Error: Could not create socket" << std::endl;
        return 1;
    }

    struct addrinfo hints, *servinfo;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(SERVER_HOST.c_str(), SERVER_PORT.c_str(), &hints, &servinfo) != 0) {
        std::cerr << "Error: Could not get address info" << std::endl;
        close(sockfd);
        return 1;
    }

    if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
        std::cerr << "Error: Could not connect to server" << std::endl;
        close(sockfd);
        freeaddrinfo(servinfo);
        return 1;
    }

    freeaddrinfo(servinfo);

    if (send(sockfd, REQUEST.c_str(), REQUEST.size(), 0) == -1) {
        std::cerr << "Error: Failed to send request" << std::endl;
        close(sockfd);
        return 1;
    }

    std::string response;
    char buffer[1024];
    int bytes_received;
    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
        response.append(buffer, bytes_received);
    }

    if (bytes_received == -1) {
        std::cerr << "Error: Failed to receive response" << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << response << std::endl;

    std::ofstream outfile("ysu.html");
    if (!outfile.is_open()) {
        std::cerr << "Error: Failed to create/open file" << std::endl;
        close(sockfd);
        return 1;
    }
    outfile << response;
    outfile.close();

    close(sockfd);
    return 0;
}

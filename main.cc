#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

using namespace std;

void send_file(const string& file_path, const string& host, int port) {
    int sockfd;
    struct sockaddr_in server_addr_in;
    struct sockaddr_in6 server_addr_in6;
    memset(&server_addr_in, 0, sizeof(server_addr_in));
    memset(&server_addr_in6, 0, sizeof(server_addr_in6));

    // Check if the host is IPv4 or IPv6
    struct sockaddr_in sa;
    struct sockaddr_in6 sa6;
    bool is_ipv6 = inet_pton(AF_INET6, host.c_str(), &(sa6.sin6_addr)) != 0;

    if (is_ipv6) {
        // Create an IPv6 socket
        sockfd = socket(AF_INET6, SOCK_STREAM, 0);

        // Prepare IPv6 address structure
        server_addr_in6.sin6_family = AF_INET6;
        server_addr_in6.sin6_port = htons(port);
        inet_pton(AF_INET6, host.c_str(), &server_addr_in6.sin6_addr);
    } else {
        // Create an IPv4 socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        // Prepare IPv4 address structure
        server_addr_in.sin_family = AF_INET;
        server_addr_in.sin_port = htons(port);
        inet_pton(AF_INET, host.c_str(), &server_addr_in.sin_addr);
    }

    // Connect to the server
    if (is_ipv6) {
        connect(sockfd, (struct sockaddr *)&server_addr_in6, sizeof(server_addr_in6));
    } else {
        connect(sockfd, (struct sockaddr *)&server_addr_in, sizeof(server_addr_in));
    }

    // Open the file to send
    ifstream file(file_path, ios::binary);
    char buffer[512];
    file.read(buffer, sizeof(buffer));
    streamsize bytes_read = file.gcount();

    // Send the file data to the server
    send(sockfd, buffer, bytes_read, 0);

    // Close the socket and file
    close(sockfd);
    file.close();
}

int main(int argc, char* argv[]) {
    // Ensure correct number of arguments
    if (argc != 5 || string(argv[1]) != "-f") {
        cerr << "Usage: sender -f <data_file> <host> <port>" << endl;
        return 1;
    }

    string file_path = argv[2];
    string host = argv[3];
    int port = atoi(argv[4]);

    // Call the send_file function to send the file
    send_file(file_path, host, port);

    return 0;
}

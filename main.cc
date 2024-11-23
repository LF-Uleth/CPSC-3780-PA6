#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>

using namespace std;

void send_file(const string& file_path, const string& host, int port) {
    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Set up the server address structure
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr);

    // Connect to the server
    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Open the file to send
    ifstream file(file_path, ios::binary);

    // Read the file and send it in chunks (up to 512 bytes)
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
    string file_path = argv[2];
    string host = argv[3];
    int port = atoi(argv[4]);

    // Call the send_file function to send the file
    send_file(file_path, host, port);

    return 0;
}

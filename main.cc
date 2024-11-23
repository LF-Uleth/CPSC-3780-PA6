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

    //Check if ipv4 or 6
    struct sockaddr_in sa;
    struct sockaddr_in6 sa6;
    bool is_ipv6 = inet_pton(AF_INET6, host.c_str(), &(sa6.sin6_addr)) != 0;

    if (is_ipv6) { //ipv6
        sockfd = socket(AF_INET6, SOCK_STREAM, 0);
        server_addr_in6.sin6_family = AF_INET6;
        server_addr_in6.sin6_port = htons(port);
        inet_pton(AF_INET6, host.c_str(), &server_addr_in6.sin6_addr);
        
    } else { //ipv4 
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        server_addr_in.sin_family = AF_INET;
        server_addr_in.sin_port = htons(port);
        inet_pton(AF_INET, host.c_str(), &server_addr_in.sin_addr);
    }

    //connect based on ipv4 or 6
    if (is_ipv6) {
        connect(sockfd, (struct sockaddr *)&server_addr_in6, sizeof(server_addr_in6)); //6
    } else {
        connect(sockfd, (struct sockaddr *)&server_addr_in, sizeof(server_addr_in)); //4
    }

    //get file
    ifstream file(file_path, ios::binary);
    char buffer[512];
    file.read(buffer, sizeof(buffer));
    streamsize bytes_read = file.gcount();

    //send file
    send(sockfd, buffer, bytes_read, 0);

    //close soceket + file
    close(sockfd);
    file.close();
}

int main(int argc, char* argv[]) {
    string file_path = argv[2];
    string host = argv[3];
    int port = atoi(argv[4]);
    
    send_file(file_path, host, port);

    return 0;
}

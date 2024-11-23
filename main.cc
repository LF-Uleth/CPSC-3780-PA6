#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

using namespace std;

void send_file(const string& file_path, const string& host, int port, bool use_ipv6) {
    int sockfd;
    struct sockaddr_in server_addr_in;
    struct sockaddr_in6 server_addr_in6;
    memset(&server_addr_in, 0, sizeof(server_addr_in));
    memset(&server_addr_in6, 0, sizeof(server_addr_in6));

    if (use_ipv6) { //6
        sockfd = socket(AF_INET6, SOCK_STREAM, 0);
        server_addr_in6.sin6_family = AF_INET6; // set addr
        server_addr_in6.sin6_port = htons(port);
        inet_pton(AF_INET6, host.c_str(), &server_addr_in6.sin6_addr);
        
        connect(sockfd, (struct sockaddr *)&server_addr_in6, sizeof(server_addr_in6));
        
    } else { //4
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        server_addr_in.sin_family = AF_INET; //set addr
        server_addr_in.sin_port = htons(port);
        inet_pton(AF_INET, host.c_str(), &server_addr_in.sin_addr);
        
        connect(sockfd, (struct sockaddr *)&server_addr_in, sizeof(server_addr_in));
    }

    //get file
    ifstream file(file_path, ios::binary);
    char buffer[512];
    file.read(buffer, sizeof(buffer));
    streamsize bytes_read = file.gcount();

    //send file
    send(sockfd, buffer, bytes_read, 0);

    //close file and socket
    close(sockfd);
    file.close();
}

int main(int argc, char* argv[]) {
    bool use_ipv6 = false; //default 4
    string file_path;
    string host;
    int port;
    int i = 1;

   
    if (string(argv[i]) == "-f") {
        file_path = argv[++i];  //set file path
    } 
   
    if (string(argv[i]) == "-6") { //check if ipv6
        use_ipv6 = true;
        i++;  // Skip the -6 flag
    }

    // set host + port
    host = argv[i++];
    port = atoi(argv[i]);
    
    send_file(file_path, host, port, use_ipv6);

    return 0;
}

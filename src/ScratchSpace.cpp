#include <iostream>
#include <string>

#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int listening_socket{0};
    listening_socket = socket(AF_INET, SOCK_STREAM, 0);

    uint16_t port = 10'000;
    struct sockaddr_in socket_address = {
        .sin_family = AF_INET,
        .sin_port = htons(port)
    };

    std::string ip_addr{"127.0.0.1"};
    inet_pton(AF_INET, ip_addr.c_str(), &socket_address.sin_addr);

    bind(listening_socket, (struct sockaddr*)&socket_address, sizeof(socket_address));
    listen(listening_socket, 50);

    while (true)
    {
        struct sockaddr_in client_address;
        socklen_t client_address_size;
        int client_socket = accept(listening_socket, (struct sockaddr*)&client_address, &client_address_size);

        
    }
}

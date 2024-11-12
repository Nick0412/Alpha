#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <string.h>
#include <vector>

int main()
{
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);

    int n;
    unsigned int m = sizeof(n);
    getsockopt(listen_sock,SOL_SOCKET,SO_RCVBUF,(void *)&n, &m);

    std::cout << "Receive buffer: " << n << "\n";

    if (listen_sock == -1)
    {
        std::cout << "Error on socket creation " << strerror(errno) << "\n";
    }

    std::cout << "Listening Sock FD int: " << listen_sock << "\n";
    // std::cout << "Listening Sock FD (real?): " << listen_sock << "\n";

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(64321);
    int res = inet_aton("127.0.0.1", &addr.sin_addr);

    if (res == 0)
    {
        std::cout << "Error on address conversion" << strerror(errno) << "\n";
    }

    int optval = 1;
    int res2 = setsockopt(listen_sock, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    if (res2 == -1)
    {
        std::cout << "Error on socket option setting " << strerror(errno) << "\n";
    }

    const int enable = 1;
    if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        std::cout << "Error on sock opt reuse addr " << strerror(errno) << "\n";
    }

    int res_bind = bind(listen_sock, (struct sockaddr*)&addr, sizeof(addr));

    if (res_bind == -1)
    {
        std::cout << "Error on bind " << strerror(errno) << "\n";
    }

    int res_listen = listen(listen_sock, 10);

    if (res_listen == -1)
    {
        std::cout << "Error on listen " << strerror(errno) << "\n";
    }

    std::vector<int> handles;

    while (true)
    {
        struct sockaddr_in client_addr;
        memset(&client_addr, 0, sizeof(addr));
        socklen_t client_addr_size = sizeof(client_addr);
        std::cout << "Waiting to accept connection\n";
        int real_data_sock = accept(listen_sock, (struct sockaddr*)&client_addr, &client_addr_size);

        if (real_data_sock == -1)
        {
            std::cout << "Error on accept " << strerror(errno) << "\n";
        }

        handles.push_back(real_data_sock);

        std::cout << "Data Sock FD int: " << real_data_sock << "\n";

        struct sockaddr_in address_of_real_data;
        socklen_t size_2 = sizeof(address_of_real_data);
        getsockname(real_data_sock, (struct sockaddr*)&address_of_real_data, &size_2);

        std::cout << "Client address: " << inet_ntoa(client_addr.sin_addr) << "\n" <<
                    "Client port: " << ntohs(client_addr.sin_port) << "\n" <<
                    "Local address: " << inet_ntoa(address_of_real_data.sin_addr) << "\n" <<
                    "Local port: " << ntohs(address_of_real_data.sin_port) << "\n";

        int x;
        std::cin >> x;
        if (x == 0)
        {
            break;
        }
        else
        {
            continue;
        }
    }

    for (auto sock : handles)
    {
        close(sock);
    }
    close(listen_sock);
}
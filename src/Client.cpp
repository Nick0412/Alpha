#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <string.h>

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
    {
        std::cout << "Error on socket creation " << strerror(errno) << "\n";
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(64321);
    int res1 = inet_aton("127.0.0.1", &addr.sin_addr);

    if (res1 == 0)
    {
        std::cout << "Error on address convert " << strerror(errno) << "\n";
    }

    int con_res = connect(sock, (struct sockaddr*)&addr, sizeof(addr));

    if (con_res == -1)
    {
        std::cout << "Error on connection " << strerror(errno) << "\n";
    }

    struct sockaddr_in addr2;
    socklen_t size2 = sizeof(addr2);
    int get_res = getsockname(sock, (struct sockaddr*)&addr2, &size2);

    if (get_res == -1)
    {
        std::cout << "Error on get res " << strerror(errno) << "\n";
    }

    struct sockaddr_in addr3;
    socklen_t size3 = sizeof(addr3);
    int peer_res = getpeername(sock, (struct sockaddr*)&addr3, &size3);

    if (peer_res == -1)
    {
        std::cout << "Error on peer res " << strerror(errno) << "\n";
    }

    std::cout << "Garbage address: " << inet_ntoa(addr.sin_addr) << "\n" <<
                 "Garbage port: " << ntohs(addr.sin_port) << "\n" << "\n" <<
                 "MY address " << inet_ntoa(addr2.sin_addr) << "\n" <<
                 "MY port " << ntohs(addr2.sin_port) << "\n" << "\n" <<
                 "PEER address " << inet_ntoa(addr3.sin_addr) << "\n" <<
                 "PEER port " << ntohs(addr3.sin_port) << "\n";

    int x;
    std::cin >> x;

    close(sock);
}
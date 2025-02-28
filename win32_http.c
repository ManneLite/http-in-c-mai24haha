#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#pragma comment(lib, "Ws2_32.lib")

#define true 1
#define false 0
typedef uint64_t u64;

int main(void)
{
    // NOTE(manuel): In windows, we need to do this in order to access tcp functionality
    WSADATA wsa_data;
    if(WSAStartup(MAKEWORD(2,2), &wsa_data)) { return 1; };

    struct sockaddr_in address = {};
    address.sin_port = htons(3000); // sin_port type is u16 (short)
    address.sin_addr.s_addr = htonl(INADDR_ANY); // sin_addr.s_addr type is u32 (long)
    address.sin_family = AF_INET;

    // Finns den typ av dörr jag vill ha?
    u64 server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(server_socket != INVALID_SOCKET)
    {
        // Sätt in dörren i en byggnad
        if(bind(server_socket, (struct sockaddr *)&address, sizeof(address)) != SOCKET_ERROR)
        {
            // Ställ dig vid dörren och invänta meddelanden
            if(listen(server_socket, 10) != SOCKET_ERROR)
            {

                #define RECV_BUFFER_SIZE 512
                char buffer[RECV_BUFFER_SIZE];
                const char *hello = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: 63\r\nConnection: close\r\n\r\n<!DOCTYPE html><html><body><h1>Hello, World!</h1></body></html>";

                int address_length = sizeof(address);
                u64 new_socket;

                while(true)
                {
                    puts("===== WAITING FOR CONNECTION =====");

                    new_socket = accept(server_socket, (struct sockaddr *)&address, 
                                        &address_length);

                    recv(new_socket, buffer, RECV_BUFFER_SIZE, 0);
                    printf("Message they sent:\n%s\n", buffer);

                    send(new_socket, hello, strlen(hello), 0);

                    shutdown(new_socket, SD_SEND);
                    closesocket(new_socket);
                }
            }

        }
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}

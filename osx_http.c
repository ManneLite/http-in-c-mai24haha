#include <sys/socket.h>
#include <arpa/inet.h>

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define true 1
#define false 0
typedef uint64_t u64;


int main(void)
{
    struct sockaddr_in address = {};
    address.sin_port = htons(3000); // sin_port type is u16 (short)
    address.sin_addr.s_addr = htonl(INADDR_ANY); // sin_addr.s_addr type is u32 (long)
    address.sin_family = AF_INET;

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(server_socket != -1)
    {
        if(bind(server_socket, (struct sockaddr *)&address, sizeof(address)) != -1)
        {
            if(listen(server_socket, 10) != -1)
            {
                #define RECV_BUFFER_SIZE 512
                char buffer[RECV_BUFFER_SIZE];
                const char *hello = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: 63\r\nConnection: close\r\n\r\n<!DOCTYPE html><html><body><h1>Hello, World!</h1></body></html>";

                int address_length = sizeof(address);
                int new_socket;

                while(true)
                {
                    puts("===== WAITING FOR CONNECTION =====");

                    new_socket = accept(server_socket, (struct sockaddr *)&address, 
                                        &address_length);

                    recv(new_socket, buffer, RECV_BUFFER_SIZE, 0);
                    printf("Message they sent:\n%s\n", buffer);

                    send(new_socket, hello, strlen(hello), 0);

                    shutdown(new_socket, SHUT_WR);
                    close(new_socket);
                }
            }
        }
    }

    close(server_socket);
    return 0;
}

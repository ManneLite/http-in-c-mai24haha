#include "http.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <memoryapi.h>

#include "win32_http.h"
#include "http.c"

#pragma comment(lib, "Ws2_32.lib")

static b32
win32_server_init_bind_and_listen(Win32_Server *server)
{
    b32 result = false;

    server->address.sin_family = server->domain;
    server->address.sin_port = htons(server->port);
    server->address.sin_addr.s_addr = htonl(server->addr_interface);

    server->socket = socket(server->domain, server->service, server->protocol);

    if(server->socket != INVALID_SOCKET)
    {
        if(bind(server->socket, (struct sockaddr *)&server->address,
                sizeof(server->address)) != SOCKET_ERROR)
        {
            if(listen(server->socket, server->backlog) != SOCKET_ERROR)
            {
                result = true;
            }
            else
            {
                fprintf(stderr, "ERROR: failed to start listening...\n");
            }
        }
        else
        {
            fprintf(stderr, "ERROR: failed to bind socket...\n");
        }
    }
    else
    {
        fprintf(stderr, "ERROR: failed to connect socket...\n");
    }

    return result;
}

static void
win32_server_launch(Win32_Server server)
{

    #define RECV_BUFFER_SIZE 2048
    char *buffer = VirtualAlloc(0, RECV_BUFFER_SIZE, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

    s32 address_length = sizeof(server.address);
    s64 new_socket;

    while(true)
    {

        puts("===== WAITING FOR CONNECTION =====");
        new_socket = accept(server.socket, (struct sockaddr *) &server.address, 
                                &address_length);
        recv(new_socket, buffer, RECV_BUFFER_SIZE, 0);

        char *response = http_handle_request(buffer);

        send(new_socket, response, strlen(response), 0);
        shutdown(new_socket, SD_SEND);
        closesocket(new_socket);
    }
}

s32 main() 
{


    WSADATA wsa_data;
    if(WSAStartup(MAKEWORD(2,2), &wsa_data) == 0)
    {
        Win32_Server server = 
        {
            .domain = AF_INET,
            .service = SOCK_STREAM,
            .protocol = 0, 
            .addr_interface = INADDR_ANY, 
            .port = 3000, 
            .backlog = 10,
        };

        if(win32_server_init_bind_and_listen(&server))
        {
            win32_server_launch(server);
        }
        closesocket(server.socket);
    }
    WSACleanup();
    return 0;
}

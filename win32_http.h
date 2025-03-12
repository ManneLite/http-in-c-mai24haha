#if !defined(WIN32_HTTP_H)
typedef u64 QWORD;

typedef struct Win32_Server
{
    u32 domain;
    u32 service;
    u32 protocol;
    u32 addr_interface;
    u16 port;
    u32 backlog;

    u64 socket;

    struct sockaddr_in address;
} Win32_Server;

#define WIN32_HTTP_H
#endif

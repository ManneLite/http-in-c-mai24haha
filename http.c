const char *not_found = "HTTP/1.1 404 NOT FOUND\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: 91\r\nConnection: close\r\n\r\n<!DOCTYPE html><html><body><h1>The resource you requested was not found!</h1></body></html>";
const char *hello = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: 63\r\nConnection: close\r\n\r\n<!DOCTYPE html><html><body><h1>Hello, World!</h1></body></html>";
const char *a = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: 63\r\nConnection: close\r\n\r\n<!DOCTYPE html><html><body><h1>Hello, AAAAA!</h1></body></html>";
const char *b = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: 63\r\nConnection: close\r\n\r\n<!DOCTYPE html><html><body><h1>Hello, BBBBB!</h1></body></html>";

static size_t
str_len(char const *test, char const terminator)
{
    int result = 0;
    while(1)
    {
        if(*test == terminator)
        {
            break;
        }
        ++result;
        ++test;
    }

    return result;
}

static size_t
c_strlen(char const *c)
{
    return str_len(c, '\0');
}

static b32
is_method(char const *method, char const *request)
{
#define MAX_HTTP_METHOD_LENGTH 8
    b32 result = false;

    for(int i = 0; i < MAX_HTTP_METHOD_LENGTH; ++i, ++method, ++request)
    {
        if(*method != *request) { break; }
        if(i == c_strlen(method) - 1) { result = true; break; }
    }

    return result;
}

static char const *
get_method_name(HTTP_Method method)
{
    char const *names[] =
    {
        "",
        "GET",
        "POST",
        "PUT",
        "PATCH",
        "DELETE",
        "CONNECT",
        "OPTIONS",
        "TRACE"
    };
    char const *result = names[method];
    return result;
}

static HTTP_Method
http_method_from_str(char const *str)
{
    HTTP_Method method = HTTP_NONE;
    for(int i = 0; i < HTTP_METHOD_COUNT; ++i)
    {
        if(is_method(get_method_name(i), str))
        {
            method = i;
            break;
        }
    }
    return method;
}

static b32
is_route(HTTP_Request request, char const *route)
{
    b32 result = false;
    if(request.uri.len == c_strlen(route))
    {
        result = true;
        for(int i = 0; i < c_strlen(route); ++i)
        {
            if(request.uri.str[i] != route[i])
            {
                result = false;
                break;
            }
        }
    }
    return result;
}

static char *
http_handle_request(char const *request_buffer)
{
    char *result = (char *)not_found;
    printf("request content:%s\n", request_buffer);

    HTTP_Request request = {};
    request.method = http_method_from_str(request_buffer);
    request.uri.str = (char *)request_buffer + strlen(get_method_name(request.method)) + 1;
    request.uri.len = str_len(request.uri.str, ' ');

    if(request.method == HTTP_GET)
    {
        if(is_route(request, "/index.html") || is_route(request, "/"))
        {
            result = (char *)hello;
        }
        if(is_route(request, "/a"))
        {
            result = (char *)a;
        }
        if(is_route(request, "/b"))
        {
            result = (char *)b;
        }
    }

    return result;

}

#if !defined(HTTP_H)
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef s32 b32;


#define kilobytes(value) ((value)*1024LL)
#define megabytes(value) (kilobytes(value)*1024LL)
#define gigabytes(value) (megabytes(value)*1024LL)
#define terabytes(value) (gigabytes(value)*1024LL)

#define len(array) (sizeof(array)/(sizeof(array)[0]))

#define assert(expression) if(!(expression)){__builtin_trap();}

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif


typedef enum HTTP_Method
{
    HTTP_NONE,
    HTTP_GET,
    HTTP_POST,
    HTTP_PUT,
    HTTP_PATCH,
    HTTP_DELETE,
    HTTP_CONNECT,
    HTTP_OPTIONS,
    HTTP_TRACE,
    HTTP_METHOD_COUNT,
} HTTP_Method;

typedef enum HTTP_Version
{
    HTTP_VERSION_NONE,
    HTTP_ONE_ONE,
    HTTP_TWO,
    HTTP_THREE,
} HTTP_Version;

typedef struct String
{
    char *str;
    size_t len;
} String, string;

typedef struct HTTP_Request
{
    HTTP_Method method;
    string uri;
    HTTP_Version version;

} HTTP_Request;

static char *http_handle_request(char const *request);

#define HTTP_H
#endif

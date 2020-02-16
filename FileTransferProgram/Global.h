#pragma once
#define STRICT
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <windows.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_BUFSIZE 8192
#define TEXT_BUFFER 64
#define TCP 500
#define UDP 501

typedef struct _SOCKET_INFORMATION {
    OVERLAPPED Overlapped;
    SOCKET Socket;
    CHAR Buffer[DATA_BUFSIZE];
    WSABUF DataBuf;
    DWORD BytesSEND;
    DWORD BytesRECV;
} SOCKET_INFORMATION, * LPSOCKET_INFORMATION;

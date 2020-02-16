#include "ConnectionManager.h"

SOCKET AcceptSocket;
WSAEVENT UDPReceiveEvent;
SOCKET* ListenSocket;

DWORD WINAPI RunServer(LPVOID lpParameter)
{
    WSADATA wsaData;
    SOCKADDR_IN InternetAddr;
    INT Ret;
    HANDLE ThreadHandle;
    DWORD ThreadId;
    WSAEVENT AcceptEvent;
    char msg[63];

    Settings* s = (Settings*)lpParameter;
    int protocol = s->protocol;

    WSACleanup();

    //Start winsock
    if ((Ret = WSAStartup(0x0202, &wsaData)) != 0)
    {
        OutputDebugString("WSAStartup failed with error \n");
        WSACleanup();
        return FALSE;
    }

    //Create socket
    ListenSocket = (SOCKET*)malloc(sizeof(SOCKET*));
    if (CreateSocket(ListenSocket, protocol) != 0)
    {
        OutputDebugString("Can't create socket \n");
        free(ListenSocket);
        WSACleanup();
        return FALSE;
    }

    //Fill addr structure
    InternetAddr = FillAddrStructure(s);

    //Bind ip address and port to socket
    if (bind(*ListenSocket, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
    {
        OutputDebugString("bind() failed with error \n");
        free(ListenSocket);
        WSACleanup();
        return FALSE;
    }
    


    //OutputDebugString(GetInitializationMessage(s));

    if (protocol == TCP)
    {
        char message[] = "Running a TCP server";
        SendOutputMessage(s->hwnd_serverout, message);

        //Open a listen socket
        if (listen(*ListenSocket, 1))
        {
            OutputDebugString("listen() failed with error \n");
            free(ListenSocket);
            WSACleanup();
            return FALSE;
        }

        if ((AcceptEvent = WSACreateEvent()) == WSA_INVALID_EVENT)
        {
            OutputDebugString("WSACreateEvent() failed with error \n");
            free(ListenSocket);
            WSACleanup();
            return FALSE;
        }

        // Create a worker thread to service completed I/O requests. 
        if ((ThreadHandle = CreateThread(NULL, 0, TCPWorkerThread, (LPVOID)AcceptEvent, 0, &ThreadId)) == NULL)
        {
            OutputDebugString("CreateThread failed with error \n");
            free(ListenSocket);
            WSACleanup();
            return FALSE;
        }

        while (TRUE)
        {
            OutputDebugString("Accepting connections...\n");
            AcceptSocket = accept(*ListenSocket, NULL, NULL);
            OutputDebugString("Reading connections...\n");

            if (WSASetEvent(AcceptEvent) == FALSE)
            {
                OutputDebugString("WSASetEvent failed with error %d\n");
                free(ListenSocket);
                WSACleanup();
                return FALSE;
            }
        }

        return TRUE;
    }
    
    if (protocol == UDP) 
    {
 /*       SOCKADDR_IN client;
        int client_len = sizeof(client);*/

        char message[] = "Running a UDP server";
        SendOutputMessage(s->hwnd_serverout, message);

        // Create a worker thread to service completed I/O requests. 
        if ((ThreadHandle = CreateThread(NULL, 0, UDPWorkerThread, ListenSocket, 0, &ThreadId)) == NULL)
        {
            OutputDebugString("CreateThread failed with error \n");
            free(ListenSocket);
            WSACleanup();
            return FALSE;
        }

    }

    return TRUE;
}

DWORD WINAPI TCPWorkerThread(LPVOID lpParameter)
{
    DWORD Flags;
    LPSOCKET_INFORMATION SocketInfo;
    WSAEVENT EventArray[1];
    DWORD Index;
    DWORD RecvBytes;

    // Save the accept event in the event array.

    EventArray[0] = (WSAEVENT)lpParameter;

    while (TRUE)
    {
        // Wait for accept() to signal an event and also process WorkerRoutine() returns.

        while (TRUE)
        {
            Index = WSAWaitForMultipleEvents(1, EventArray, FALSE, WSA_INFINITE, TRUE);

            if (Index == WSA_WAIT_FAILED)
            {
                OutputDebugString("WSAWaitForMultipleEvents failed with error %d\n");
                return FALSE;
            }

            if (Index != WAIT_IO_COMPLETION)
            {
                // An accept() call event is ready - break the wait loop
                break;
            }
        }

        WSAResetEvent(EventArray[Index - WSA_WAIT_EVENT_0]);

        // Create a socket information structure to associate with the accepted socket.

        if ((SocketInfo = (LPSOCKET_INFORMATION)GlobalAlloc(GPTR,
            sizeof(SOCKET_INFORMATION))) == NULL)
        {
            OutputDebugString("GlobalAlloc() failed with error %d\n");
            return FALSE;
        }

        // Fill in the details of our accepted socket.

        SocketInfo->Socket = AcceptSocket;
        ZeroMemory(&(SocketInfo->Overlapped), sizeof(WSAOVERLAPPED));
        SocketInfo->BytesSEND = 0;
        SocketInfo->BytesRECV = 0;
        SocketInfo->DataBuf.len = DATA_BUFSIZE;
        SocketInfo->DataBuf.buf = SocketInfo->Buffer;

        Flags = 0;
        if (WSARecv(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &RecvBytes, &Flags,
            &(SocketInfo->Overlapped), TCPWorkerRoutine) == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSA_IO_PENDING)
            {
                OutputDebugString("WSARecv() failed with error %d\n");
                return FALSE;
            }
        }

        OutputDebugString("Reading connections...\n");
    }

    return TRUE;
}

void CALLBACK TCPWorkerRoutine(DWORD Error, DWORD BytesTransferred,
    LPWSAOVERLAPPED Overlapped, DWORD InFlags)
{
    DWORD SendBytes, RecvBytes;
    DWORD Flags;

    // Reference the WSAOVERLAPPED structure as a SOCKET_INFORMATION structure
    LPSOCKET_INFORMATION SI = (LPSOCKET_INFORMATION)Overlapped;

    if (Error != 0)
    {
        OutputDebugString("I/O operation failed with error %d\n");
    }

    if (BytesTransferred == 0)
    {
        OutputDebugString("Closing socket %d\n");
    }

    if (Error != 0 || BytesTransferred == 0)
    {
        closesocket(SI->Socket);
        GlobalFree(SI);
        return;
    }

    // Check to see if the BytesRECV field equals zero. If this is so, then
    // this means a WSARecv call just completed so update the BytesRECV field
    // with the BytesTransferred value from the completed WSARecv() call.

    if (SI->BytesRECV == 0)
    {
        SI->BytesRECV = BytesTransferred;
        SI->BytesSEND = 0;
    }
    else
    {
        SI->BytesSEND += BytesTransferred;
    }

    if (SI->BytesRECV > SI->BytesSEND)
    {
        // Post another WSASend() request.
        // Since WSASend() is not gauranteed to send all of the bytes requested,
        // continue posting WSASend() calls until all received bytes are sent.

        ZeroMemory(&(SI->Overlapped), sizeof(WSAOVERLAPPED));

        SI->DataBuf.buf = SI->Buffer + SI->BytesSEND;
        SI->DataBuf.len = SI->BytesRECV - SI->BytesSEND;

        if (WSASend(SI->Socket, &(SI->DataBuf), 1, &SendBytes, 0,
            &(SI->Overlapped), TCPWorkerRoutine) == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSA_IO_PENDING)
            {
                OutputDebugString("WSASend() failed with error %d\n");
                return;
            }
        }
    }
    else
    {
        SI->BytesRECV = 0;

        // Now that there are no more bytes to send post another WSARecv() request.

        Flags = 0;
        ZeroMemory(&(SI->Overlapped), sizeof(WSAOVERLAPPED));

        SI->DataBuf.len = DATA_BUFSIZE;
        SI->DataBuf.buf = SI->Buffer;

        if (WSARecv(SI->Socket, &(SI->DataBuf), 1, &RecvBytes, &Flags,
            &(SI->Overlapped), TCPWorkerRoutine) == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSA_IO_PENDING)
            {
                OutputDebugString("WSARecv() failed with error %d\n");
                return;
            }
        }
    }
}

DWORD WINAPI UDPWorkerThread(LPVOID lpParameter)
{
    DWORD Flags;
    LPSOCKET_INFORMATION SocketInfo;
    //WSAEVENT EventArray[1];
    //DWORD Index;
    DWORD RecvBytes;
    SOCKADDR_IN  client;
    DWORD rc;
    DWORD err;
    DWORD retval = 1;

    int client_len = sizeof(client);
    Flags = 0;

    if ((SocketInfo = (LPSOCKET_INFORMATION)GlobalAlloc(GPTR,
        sizeof(SOCKET_INFORMATION))) == NULL)
    {
        OutputDebugString("GlobalAlloc() failed with error \n");
        return FALSE;
    }

    // Fill in the details of the socket
    SocketInfo->Socket = *ListenSocket;
    ZeroMemory(&(SocketInfo->Overlapped), sizeof(WSAOVERLAPPED));
    SocketInfo->BytesSEND = 0;
    SocketInfo->BytesRECV = 0;
    SocketInfo->DataBuf.len = DATA_BUFSIZE;
    SocketInfo->DataBuf.buf = SocketInfo->Buffer;
    SocketInfo->Overlapped.hEvent = WSACreateEvent();

    while (TRUE)
    {
        rc = WSARecvFrom(*ListenSocket, &(SocketInfo->DataBuf),
            1, &(SocketInfo->BytesRECV), &Flags, (SOCKADDR*)&client,
            &client_len, &(SocketInfo->Overlapped), NULL);

        if (rc != 0) {
            err = WSAGetLastError();
            if (err != WSA_IO_PENDING) {
                OutputDebugString("WSARecvFrom failed with error\n");
                WSACloseEvent(SocketInfo->Overlapped.hEvent);
                closesocket(SocketInfo->Socket);
                WSACleanup();
                return 1;
            }
            else {
                rc = WSAWaitForMultipleEvents(1, &SocketInfo->Overlapped.hEvent, TRUE, WSA_INFINITE, TRUE);
                if (rc == WSA_WAIT_FAILED) {
                    OutputDebugString("WSAWaitForMultipleEvents failed with error.");
                    retval = 1;
                }

                rc = WSAGetOverlappedResult(SocketInfo->Socket, &SocketInfo->Overlapped, &SocketInfo->BytesRECV,
                    FALSE, &Flags);
                if (rc == FALSE) {
                    OutputDebugString("WSArecvFrom failed with error.\n");
                    retval = 1;
                }
                else
                {
                    OutputDebugString("Number of received bytes = ");
                    TCHAR resp_byte[20];
                    _itoa(SocketInfo->BytesRECV, resp_byte, 10);
                    OutputDebugString(resp_byte);
                    OutputDebugString("\n");
                    OutputDebugString("Received:\n");
                    OutputDebugString(SocketInfo->Buffer);
                    OutputDebugString("\n");
                }

                OutputDebugString("Finished receiving. Closing socket.\n");
            }

        }
    }

    return TRUE;
}


DWORD CreateSocket(SOCKET* socket, int protocol)
{
    int protocol_flag = IPPROTO_TCP;
    int type = SOCK_STREAM;

    if (protocol == UDP)
    {
        protocol_flag = IPPROTO_UDP;
        type = SOCK_DGRAM;
    }

    if ((*socket = WSASocket(AF_INET, type, protocol_flag, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
        OutputDebugString("Can't create socket!");
        return 1;
    }

    return 0;
}

SOCKADDR_IN FillAddrStructure(Settings* setting)
{
    SOCKADDR_IN addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(setting->ip_address);
    addr.sin_port = htons(setting->port_number);

    return addr;
}

const char* GetInitializationMessage(Settings* s)
{
    char protocol[64];

    switch (s->protocol)
    {
    case (TCP):
        strcpy(protocol, "Established a TCP connection.\n");
        break;
    case UDP:
        strcpy(protocol, "Ready to receive UDP packets.\n");
        break;
    default:
        strcpy(protocol, "Server has somehow connected to neither UDP nor TCP.\n");
        break;
    }

    return protocol;
}

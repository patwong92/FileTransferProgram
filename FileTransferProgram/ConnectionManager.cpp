/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: RunServer.cpp - Provides functions to receive TCP/UDP data
--
-- PROGRAM: FileTransferProgram
--
-- FUNCTIONS:
-- DWORD WINAPI RunServer(LPVOID lpParameter)
-- DWORD WINAPI TCPWorkerThread(LPVOID lpParameter)
-- DWORD WINAPI UDPWorkerThread(LPVOID lpParameter)
-- void CALLBACK TCPWorkerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags)
-- DWORD CreateSocket(SOCKET* socket, int protocol)
-- SOCKADDR_IN FillAddrStructure(Settings* setting)
-- DWORD WINAPI WaitingForTCPConnections(LPVOID lpParameter)
--
-- DATE: February 15, 2020
--
-- REVISIONS: February 18, 2020
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- NOTES:
-- This cpp file provides functions to receive TCP packets or UDP datagrams. The model used to handle TCP packets is the
-- completion I/O mode. The model used to handle UDP datagrams is the overlapped I/O model. Both models are needed to
-- enhance the data output, avoid extra copies, and allow flexibility in timing relationship between the socket and the 
-- network. 
----------------------------------------------------------------------------------------------------------------------*/
#include "ConnectionManager.h"

SOCKET AcceptSocket;
WSAEVENT UDPReceiveEvent;
SOCKET* ListenSocket;
HWND serveroutput;
char* write_file_path;

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: RunServer
--
-- DATE: February 15, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: DWORD WINAPI RunServer(LPVOID lpParameter)
--			LPVOID lpParameter - Long pointer to a parameter. In this case, the parameter is the Settings structure.
--
-- RETURNS: TRUE or 1 if the function call is successful
--			FALSE or 0 if the function call encounters an error.
--
-- NOTES:
-- This function identifies the protocol and runs the associated thread. If the protocol is TCP, then the function
-- calls the TCPWorkerThread. If the protocol is UDP, then the function calls the UDPWorkerThread.
----------------------------------------------------------------------------------------------------------------------*/
DWORD WINAPI RunServer(LPVOID lpParameter)
{
    WSADATA wsaData;
    SOCKADDR_IN InternetAddr;
    INT Ret;
    HANDLE ThreadHandle;
    DWORD ThreadId;
    WSAEVENT AcceptEvent;
    TCHAR msg[63];

    Settings* s = (Settings*)lpParameter;
    int protocol = s->protocol;
    serveroutput = s->hwnd_serverout;
    write_file_path = s->save_file_path;

    WSACleanup();

    //Start winsock
    if ((Ret = WSAStartup(0x0202, &wsaData)) != 0)
    {
        TCHAR error_msg[] = "WSAStartup failed with error.";
        SendOutputMessage(serveroutput, error_msg);
        WSACleanup();
        return FALSE;
    }

    //Create socket
    ListenSocket = (SOCKET*)malloc(sizeof(SOCKET*));
    if (CreateSocket(ListenSocket, protocol) != 0)
    {
        TCHAR error_msg[] = "Can't create socket.";
        SendOutputMessage(serveroutput, error_msg);

        free(ListenSocket);
        WSACleanup();
        return FALSE;
    }

    //Fill addr structure
    InternetAddr = FillAddrStructure(s);

    //Bind ip address and port to socket
    if (bind(*ListenSocket, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
    {
        TCHAR error_msg[] = "Can't bind ip address and port to socket";
        SendOutputMessage(serveroutput, error_msg);

        free(ListenSocket);
        WSACleanup();
        return FALSE;
    }

    //OutputDebugString(GetInitializationMessage(s));

    if (protocol == TCP)
    {
        TCHAR connect_msg[] = "Successful TCP server connection";
        TCHAR reading_msg[] = "Server is reading connections...";

        SendOutputMessage(serveroutput, connect_msg);

        //Open a listen socket
        if (listen(*ListenSocket, 1))
        {
            TCHAR error_msg[] = "Listen failed!";
            SendOutputMessage(serveroutput, error_msg);

            free(ListenSocket);
            WSACleanup();
            return FALSE;
        }

        if ((AcceptEvent = WSACreateEvent()) == WSA_INVALID_EVENT)
        {
            TCHAR error_msg[] = "Can't call WSACreateEvent()!";
            SendOutputMessage(serveroutput, error_msg);

            free(ListenSocket);
            WSACleanup();
            return FALSE;
        }

        // Create a worker thread to service completed I/O requests. 
        if ((ThreadHandle = CreateThread(NULL, 0, TCPWorkerThread, (LPVOID)AcceptEvent, 0, &ThreadId)) == NULL)
        {
            TCHAR error_msg[] = "Can't create accept event thread!";
            SendOutputMessage(serveroutput, error_msg);

            free(ListenSocket);
            WSACleanup();
            return FALSE;
        }

        while (TRUE)
        {
            AcceptSocket = accept(*ListenSocket, NULL, NULL);
            SendOutputMessage(serveroutput, reading_msg);

            if (WSASetEvent(AcceptEvent) == FALSE)
            {
                TCHAR error_msg[] = "Can't set WSA event!";
                SendOutputMessage(serveroutput, error_msg);

                free(ListenSocket);
                WSACleanup();
                return FALSE;
            }
        }

        return TRUE;
    }
    
    if (protocol == UDP) 
    {
        TCHAR message[] = "Successful UDP server connection";
        SendOutputMessage(serveroutput, message);

        // Create a worker thread to service completed I/O requests. 
        if ((ThreadHandle = CreateThread(NULL, 0, UDPWorkerThread, ListenSocket, 0, &ThreadId)) == NULL)
        {
            TCHAR error_msg[] = "Can't create UDP thread!";
            SendOutputMessage(serveroutput, error_msg);

            free(ListenSocket);
            WSACleanup();
            return FALSE;
        }

    }

    return TRUE;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: TCPWorkerThread
--
-- DATE: February 15, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: DWORD WINAPI TCPWorkerThread(LPVOID lpParameter)
--			LPVOID lpParameter - Long pointer to a parameter. In this case, the parameter is the WSAEVENT structure.
--
-- RETURNS: TRUE or 1 if the function call is successful
--			FALSE or 0 if the function call encounters an error.
--
-- NOTES:
-- This thread function continuously waits, and accepts for any events. If there is an event, then the function calls
-- in a TCPWorkerRoutine to handle the data transfer.
----------------------------------------------------------------------------------------------------------------------*/
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
                TCHAR error_msg[] = "WSAWaitForMultipleEvents failed with error!";
                SendOutputMessage(serveroutput, error_msg);
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
            TCHAR error_msg[] = "GlobalAlloc failed with error!";
            SendOutputMessage(serveroutput, error_msg);
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
        

        TCHAR receive_msg[] = "Server received:";
        SendOutputMessage(serveroutput, receive_msg);
        if (WSARecv(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &RecvBytes, &Flags,
            &(SocketInfo->Overlapped), TCPWorkerRoutine) == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSA_IO_PENDING)
            {
                TCHAR error_msg[] = "WSARecv() failed with error!";
                SendOutputMessage(serveroutput, error_msg);
                return FALSE;
            }
        }

        //TCHAR reading_msg[] = "Finished reading connecton...";
        //SendOutputMessage(serveroutput, reading_msg);
        
    }


    return TRUE;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: TCPWorkerRoutine
--
-- DATE: February 15, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: void CALLBACK TCPWorkerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags)
--			DWORD Error - Error code
--			DWORD BytesTransferred - Bytes transferred to the server
--			LPWSAOVERLAPPED overlapped - Long pointer to the overlapped structure
--			DWORD InFlags - Flags (unused)
--
-- RETURNS: TRUE or 1 if the function call is successful
--			FALSE or 0 if the function call encounters an error.
--
-- NOTES:
-- This thread function recursively receives bytes from the socket until there is no more data to receive.
----------------------------------------------------------------------------------------------------------------------*/
void CALLBACK TCPWorkerRoutine(DWORD Error, DWORD BytesTransferred,
    LPWSAOVERLAPPED Overlapped, DWORD InFlags)
{
    DWORD SendBytes, RecvBytes;
    DWORD Flags;

    // Reference the WSAOVERLAPPED structure as a SOCKET_INFORMATION structure
    LPSOCKET_INFORMATION SI = (LPSOCKET_INFORMATION)Overlapped;

    if (Error != 0)
    {
        TCHAR error_msg[] = "I/O operation failed with error!";
        SendOutputMessage(serveroutput, error_msg);
    }

    if (BytesTransferred == 0)
    {
        OutputDebugString("Closing socket...");
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

        /*SendOutputMessage(serveroutput, SI->Buffer);*/ //Output text 
        
        if (WSASend(SI->Socket, &(SI->DataBuf), 1, &SendBytes, 0,
            &(SI->Overlapped), TCPWorkerRoutine) == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSA_IO_PENDING)
            {
                TCHAR error_msg[] = "WSASend() failed with error!";
                SendOutputMessage(serveroutput, error_msg);
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
                TCHAR error_msg[] = "WSARecv() failed with error!";
                SendOutputMessage(serveroutput, error_msg);

                return;
            }
        }
    }


    if (SI->BytesRECV > 0)
    {
        TCHAR resp_msg[] = "Number of bytes received =";
        TCHAR resp_byte[20];
        _itoa(SI->BytesRECV, resp_byte, 10);
        SendOutputMessage(serveroutput, resp_msg, resp_byte);

        if (WriteFile(write_file_path, &SI->DataBuf.buf) != 0)
        {
            TCHAR error_msg[] = "Error in writing data to file!";
            SendOutputMessage(serveroutput, error_msg);
        }
    }

}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: UDPWorkerThread
--
-- DATE: February 15, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: DWORD WINAPI UDPWorkerThread(LPVOID lpParameter)
--			LPVOID lpParameter - Long pointer to a parameter. In this case, the parameter is unused.
--
-- RETURNS: TRUE or 1 if the function call is successful
--			FALSE or 0 if the function call encounters an error.
--
-- NOTES:
-- This thread function creates an overlapped event and waits for multiple events. IF there is an multiple event,
-- then the function gets the overlapped result, reads the datagram, and sends back the datagram to the source.
----------------------------------------------------------------------------------------------------------------------*/
DWORD WINAPI UDPWorkerThread(LPVOID lpParameter)
{
    DWORD Flags;
    LPSOCKET_INFORMATION SocketInfo;
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
        TCHAR error_msg[] = "GlobalAlloc() failed with error!";
        SendOutputMessage(serveroutput, error_msg);
        return FALSE;
    }

    // Fill in the details of the socket
    SocketInfo->Socket = *ListenSocket;
    ZeroMemory(&(SocketInfo->Overlapped), sizeof(WSAOVERLAPPED));
    SocketInfo->BytesSEND = 0;
    SocketInfo->BytesRECV = 0;
    SocketInfo->DataBuf.len = 65000;
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
                TCHAR error_msg[] = "WSARecvFrom failed with error!";
                SendOutputMessage(serveroutput, error_msg);

                WSACloseEvent(SocketInfo->Overlapped.hEvent);
                closesocket(SocketInfo->Socket);
                WSACleanup();
                return 1;
            }
            else {
                rc = WSAWaitForMultipleEvents(1, &SocketInfo->Overlapped.hEvent, TRUE, WSA_INFINITE, TRUE);
                if (rc == WSA_WAIT_FAILED) {
                    TCHAR error_msg[] = "WSAWaitForMultipleEvents failed with error!";
                    SendOutputMessage(serveroutput, error_msg);
                    retval = 1;
                }

                rc = WSAGetOverlappedResult(SocketInfo->Socket, &SocketInfo->Overlapped, &SocketInfo->BytesRECV,
                    FALSE, &Flags);
                if (rc == FALSE) {
                    TCHAR error_msg[] = "WSARecvFrom failed with error!";
                    SendOutputMessage(serveroutput, error_msg);
                    retval = 1;
                }
                else
                {
                    TCHAR receive_msg1[] = "Number of bytes received =";
                    TCHAR receive_msg2[] = "Server received:";
                    TCHAR resp_byte[20];
                    _itoa(SocketInfo->BytesRECV, resp_byte, 10);

                    SendOutputMessage(serveroutput, receive_msg1, resp_byte);
                    //SendOutputMessage(serveroutput, receive_msg2);
                    //SendOutputMessage(serveroutput, SocketInfo->Buffer);

                    if (WriteFile(write_file_path, &SocketInfo->DataBuf.buf) != 0)
                    {
                        TCHAR error_msg[] = "Error in writing data to file!";
                        SendOutputMessage(serveroutput, error_msg);
                    }

                    //Echo response to client
                    WSASendTo(*ListenSocket, &(SocketInfo->DataBuf),
                        1, &(SocketInfo->BytesRECV), Flags, (SOCKADDR*)&client,
                        client_len, &(SocketInfo->Overlapped), NULL);

                }
            }

        }
    }

    return TRUE;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: CreateSocket
--
-- DATE: February 15, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: DWORD CreateSocket(SOCKET* socket, int protocol);
--			SOCKET* socket - Pointer to a SOCKET structure
--			int protocol - Protocol code
--
-- RETURNS: 0 if the function call is successful
--			1 if the function call encounters an error.
--
-- NOTES:
-- This function identifies the protocol and modifies the type for use in the WSASocket() call. If the protocol is TCP,
-- then the type is SOCK_STREAM. If the protocol is UDP, the type is SOCK_DGRAM
----------------------------------------------------------------------------------------------------------------------*/
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
        TCHAR error_msg[] = "Can't create socket!";
        SendOutputMessage(serveroutput, error_msg);
        return 1;
    }

    return 0;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: FillAddrStructure
--
-- DATE: February 15, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: SOCKADDR_IN FillAddrStructure(Settings* setting)
--			Settings* setting - Pointer to the Settings structure
--
-- RETURNS: SOCK_ADDR_IN structure with initialized data members.
--
-- NOTES:
-- This function fills in the SOCKADDR_IN structure for use in connect() in TCP.
----------------------------------------------------------------------------------------------------------------------*/
SOCKADDR_IN FillAddrStructure(Settings* setting)
{
    SOCKADDR_IN addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(setting->ip_address);
    addr.sin_port = htons(setting->port_number);

    return addr;
}


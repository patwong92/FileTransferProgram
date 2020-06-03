#pragma once
#include "Global.h"
#include "Settings.h"
#include "Widgets.h"
#include "Utilities.h"

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
DWORD WINAPI RunServer(LPVOID lpParameter);

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
DWORD WINAPI TCPWorkerThread(LPVOID lpParameter);

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
void CALLBACK TCPWorkerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags);

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
DWORD WINAPI UDPWorkerThread(LPVOID lpParameter);

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
DWORD CreateSocket(SOCKET* socket, int protocol);

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
SOCKADDR_IN FillAddrStructure(Settings* setting);

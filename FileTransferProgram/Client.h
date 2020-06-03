#pragma once

#include "Global.h"
#include "Settings.h"
#include "Utilities.h"
#include "Widgets.h"

#define MAXLEN				65000   // Maximum Buffer length
#define SEND_BUFFER			1024
#define TEXT_BUFFER			64		// Default Length

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: RunClient
--
-- DATE: February 15, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: void RunClient(Settings* s)
--			Settings* s: Settings structure to store client parameters
--
-- RETURNS: void
--
-- NOTES:
-- This function checks whether the protocol whether it is TCP or UDP and runs the associated thread. 
----------------------------------------------------------------------------------------------------------------------*/
void RunClient(Settings* s);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: TCPClient
--
-- DATE: February 15, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: DWORD WINAPI TCPClient(LPVOID lpParameter)
--			LPVOID lpParameter - Long pointer to a parameter. The parameter in this case is the Settings pointer.
--
-- RETURNS: FALSE or 0 if the thread function encounters an error
--			TRUE or 1 if the thread function succesfully works
--
-- NOTES:
-- This function calls the winsock2.h library to run a TCP client. Once the client connects to the server, the client
-- sends a file to the server X number of times, where X is the number specified in the client window. By default,
-- the client sends the file 1 time. The user can control the file content and the packet size.
----------------------------------------------------------------------------------------------------------------------*/
DWORD WINAPI TCPClient(LPVOID lpParameter);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: UDPClient
--
-- DATE: February 15, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: DWORD WINAPI UDPClient(LPVOID lpParameter)
--			LPVOID lpParameter - Long pointer to a parameter. The parameter in this case is the Settings pointer.
--
-- RETURNS: FALSE or 0 if the thread function encounters an error
--			TRUE or 1 if the thread function succesfully works
--
-- NOTES:
-- This function calls the winsock2.h library to run a UDP client. Once the client binds the port to the socket, the client
-- sends a file to the server X number of times, where X is the number specified in the client window. By default,
-- the client sends the file 1 time. The user can control the file content and the packet size.
----------------------------------------------------------------------------------------------------------------------*/
DWORD WINAPI UDPClient(LPVOID lpParameter);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: delay
--
-- DATE: February 15, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: long delay(SYSTEMTIME t1, SYSTEMTIME t2)
--			SYSTEMTIME t1 - Initial time
--			SYSTEMTIME t2 - Final time
--
-- RETURNS: Returns the time elapsed in milliseconds between the specified initial and final times.
--
-- NOTES:
-- This function calculates the difference between two times and returns the time elapsed in milliseconds.
----------------------------------------------------------------------------------------------------------------------*/
long delay(SYSTEMTIME t1, SYSTEMTIME t2);


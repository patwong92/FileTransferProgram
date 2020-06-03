/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: Client.cpp - Provides functions to send TCP/UDP data 
--
-- PROGRAM: FileTransferProgram
--
-- FUNCTIONS:
-- void RunClient(Settings* s)
-- DWORD WINAPI TCPClient(LPVOID lpParameter)
-- DWORD WINAPI UDPClient(LPVOID lpParameter)
-- long delay(SYSTEMTIME t1, SYSTEMTIME t2)
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
-- This cpp file provides functions to send TCP/UDP data. The client design for both protcols require to send data to a host
-- and receive the packet back as an acknowledgement.
----------------------------------------------------------------------------------------------------------------------*/
#include "Client.h"

HWND hwnd_clientout;

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
void RunClient(Settings* s)
{
	HANDLE ClientThreadHandle;
	DWORD ThreadId;

	int protocol = s->protocol;
	hwnd_clientout = s->hwnd_clientout;

	ClearOutputMessage(hwnd_clientout);

	if (protocol == TCP)
	{
		if ((ClientThreadHandle = CreateThread(NULL, 0, TCPClient, (LPVOID)s, 0, &ThreadId)) == NULL)
		{
			TCHAR error_msg[] = "Error in CreateThread";
			SendOutputMessage(hwnd_clientout, error_msg);
			return;
		}
	}

	if (protocol == UDP)
	{
		if ((ClientThreadHandle = CreateThread(NULL, 0, UDPClient, (LPVOID)s, 0, &ThreadId)) == NULL)
		{
			TCHAR error_msg[] = "Error in CreateThread";
			SendOutputMessage(hwnd_clientout, error_msg);
			return;
		}
	}

	return;
}

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
DWORD WINAPI TCPClient(LPVOID lpParameter)
{
	Settings* setting = (Settings*)lpParameter;

	int n, ns, bytes_to_read;
	int port, err;
	SOCKET sd;
	struct hostent* hp;
	struct sockaddr_in server;
	TCHAR* host, * bp, rbuf[SEND_BUFFER], ** pptr;
	TCHAR* sbuf = (TCHAR*)malloc(sizeof(TCHAR) * (SEND_BUFFER + 1));
	int packet_size_memory = sizeof(TCHAR) * (setting->packet_size);

	WSADATA WSAData;
	WORD wVersionRequested;

	if (sbuf != NULL)
		memset(sbuf, 0, packet_size_memory);
	else {
		TCHAR error_msg[] = "Error in initializing buffer.";
		SendOutputMessage(hwnd_clientout, error_msg);
		return FALSE;
	}

	if (ReadFile(setting->open_file_path, &sbuf, packet_size_memory) != 0)
	{
		TCHAR error_msg[] = "Please check that the file path is correct.";
		SendOutputMessage(hwnd_clientout, error_msg);
		free(sbuf);
		return FALSE;
	}

	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &WSAData);
	if (err != 0) //No usable DLL
	{
		TCHAR error_msg[] = "Error in WSAStartup!";
		SendOutputMessage(hwnd_clientout, error_msg);
		free(sbuf);
		return FALSE;
	}

	//Create the socket
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		TCHAR error_msg[] = "Error in opening socket!";
		SendOutputMessage(hwnd_clientout, error_msg);
		free(sbuf);
		return FALSE;
	}

	//Initialize and set up the address structure
	memset((TCHAR*)&server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(setting->port_number);
	if ((hp = gethostbyname(setting->ip_address)) == NULL)
	{
		TCHAR error_msg[] = "Error in calling gethostbyname!";
		SendOutputMessage(hwnd_clientout, error_msg);
		free(sbuf);
		return FALSE;
	}

	//Copy the server address
	memcpy((TCHAR*)&server.sin_addr, hp->h_addr, hp->h_length);

	//Connecting to the server
	if (connect(sd, (struct sockaddr*) & server, sizeof(server)) == -1)
	{
		TCHAR error_msg[] = "Can't connect to server!";
		SendOutputMessage(hwnd_clientout, error_msg);
		free(sbuf);
		return FALSE;
	}

	TCHAR serv_name[] = "Connected server name:";
	TCHAR ip_address[] = "Connected IP address:";

	SendOutputMessage(hwnd_clientout, serv_name, hp->h_name);
	pptr = hp->h_addr_list;
	SendOutputMessage(hwnd_clientout, ip_address, inet_ntoa(server.sin_addr));

	TCHAR send_msg[] = "Client sent:";
	SendOutputMessage(hwnd_clientout, send_msg);

	SendOutputMessage(hwnd_clientout, sbuf, setting->packet_size);
	TCHAR num_bytes_send_msg[] = "Packet size sent =";
	TCHAR num_bytes_sent[TEXT_BUFFER];
	_itoa(setting->packet_size, num_bytes_sent, 10);
	SendOutputMessage(hwnd_clientout, num_bytes_send_msg, num_bytes_sent);

	int counter = 0;

	for (int i = 0; i < setting->client_frequency; i++)
	{
		//Transmit data through the socket
		ns = send(sd, sbuf, packet_size_memory, 0);

		//TCHAR receiving_msg[] = "Client received:";
		//SendOutputMessage(hwnd_clientout, receiving_msg);

		bp = rbuf;
		bytes_to_read = SEND_BUFFER;
		TCHAR r_message[DATA_BUFSIZE];

		//client makes repeated calls to recv until no more data is expected to arrive.
		while ((n = recv(sd, bp, bytes_to_read, 0)) < SEND_BUFFER)
		{
			memset(r_message, 0, sizeof(r_message));
			strcpy(r_message, bp);
			//SendOutputMessage(hwnd_clientout, r_message);

			bp += n;
			bytes_to_read -= n;

			if (n == 0)
				break;
		}

		counter++;
	}
	
	TCHAR success_msg[] = "Number of packets successfully sent =";
	TCHAR num_bytes_success[TEXT_BUFFER];
	_itoa(counter, num_bytes_success, 10);
	SendOutputMessage(hwnd_clientout, success_msg, num_bytes_success);

	free(sbuf);
	closesocket(sd);
	WSACleanup();
	return TRUE;
}

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
DWORD WINAPI UDPClient(LPVOID lpParameter)
{
	Settings* setting = (Settings*)lpParameter;

	int	data_size = SEND_BUFFER;
	int err;
	int server_len;
	int client_len;
	int packet_size_memory = sizeof(TCHAR) * (setting->packet_size);

	SOCKET sd;
	TCHAR* sbuf = (TCHAR*) malloc(packet_size_memory); //packet size will change depending on user's preference

	struct	hostent* hp;
	struct	sockaddr_in server, client;
	SYSTEMTIME stStartTime, stEndTime;
	WSADATA stWSAData;
	WORD wVersionRequested = MAKEWORD(2, 2);

	if (sbuf != NULL)
		memset(sbuf, 0, packet_size_memory);
	else {
		TCHAR error_msg[] = "Error in initializing buffer.";
		SendOutputMessage(hwnd_clientout, error_msg);
		return FALSE;
	}

	if (ReadFile(setting->open_file_path, &sbuf, packet_size_memory) != 0)
	{
		TCHAR error_msg[] = "Please check that the file path is correct.";
		SendOutputMessage(hwnd_clientout, error_msg);
		free(sbuf);
		return FALSE;
	}

	// Initialize the DLL with version Winsock 2.2
	err = WSAStartup(wVersionRequested, &stWSAData);
	if (err != 0) //No usable DLL
	{
		TCHAR error_msg[] = "Error in WSAStartup!";
		SendOutputMessage(hwnd_clientout, error_msg);
		free(sbuf);
		return FALSE;
	}

	// Create a datagram socket
	if ((sd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
	{
		TCHAR error_msg[] = "Error in opening socket!";
		SendOutputMessage(hwnd_clientout, error_msg);
		free(sbuf);
		return FALSE;
	}

	// Store server's information
	memset((char*)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(setting->port_number);

	if ((hp = gethostbyname(setting->ip_address)) == NULL)
	{
		TCHAR error_msg[] = "Error in calling gethostbyname!";
		SendOutputMessage(hwnd_clientout, error_msg);
		free(sbuf);
		return FALSE;
	}

	memcpy((char*)&server.sin_addr, hp->h_addr, hp->h_length);

	// Bind local address to the socket
	memset((char*)&client, 0, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port = htons(0);  // bind to any available port
	client.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sd, (struct sockaddr*) & client, sizeof(client)) == -1)
	{
		TCHAR error_msg[] = "Can't bind name to socket.";
		SendOutputMessage(hwnd_clientout, error_msg);
		free(sbuf);
		return FALSE;
	}

	// Find out what port was assigned and print it
	client_len = sizeof(client);
	if (getsockname(sd, (struct sockaddr*) & client, &client_len) < 0)
	{
		TCHAR error_msg[] = "Error in calling getsockname.";
		SendOutputMessage(hwnd_clientout, error_msg);
		free(sbuf);
		return FALSE;
	}

	int counter = 0;
	// transmit data
	TCHAR send_msg[] = "Client sent:";
	SendOutputMessage(hwnd_clientout, send_msg);
	SendOutputMessage(hwnd_clientout, sbuf, setting->packet_size);

	for (int i = 0; i < setting->client_frequency; i++)
	{
		//SendOutputMessage(hwnd_clientout, sbuf, setting->packet_size);

		server_len = sizeof(server);
		if (sendto(sd, sbuf, packet_size_memory, 0, (struct sockaddr*) & server, server_len) == -1)
		{
			TCHAR error_msg[] = "Error in calling sendto!";
			SendOutputMessage(hwnd_clientout, error_msg);
			break;
		}

		TCHAR num_bytes_send_msg[] = "Number of bytes sent =";
		TCHAR num_bytes_sent[TEXT_BUFFER];
		_itoa(setting->packet_size, num_bytes_sent, 10);
		//SendOutputMessage(hwnd_clientout, num_bytes_send_msg, num_bytes_sent);

		TCHAR rbuf[MAXLEN];

		// receive data
		if (recvfrom(sd, rbuf, MAXLEN, 0, (struct sockaddr*) & server, &server_len) < 0)
		{
			TCHAR error_msg[] = "Error in calling recvfrom!";
			SendOutputMessage(hwnd_clientout, error_msg);
			break;
		}

		//TCHAR recv_msg[] = "Client received:";
		//SendOutputMessage(hwnd_clientout, recv_msg);
		/*SendOutputMessage(hwnd_clientout, rbuf);	*/

		counter++;
	}

	TCHAR success_msg[] = "Number of packets successfully sent =";
	TCHAR num_bytes_success[TEXT_BUFFER];
	_itoa(counter, num_bytes_success, 10);
	SendOutputMessage(hwnd_clientout, success_msg, num_bytes_success);

	//TCHAR check[TEXT_BUFFER];
	//_itoa(counter, check, 10);
	//SendOutputMessage(hwnd_clientout, check);

	free(sbuf);
	closesocket(sd);
	WSACleanup();

	return TRUE;
}

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
-- RETURNS: Returns the time elapsed between the specified initial and final times.
--
-- NOTES:
-- This function calculates the difference between two times and returns the time elapsed.
----------------------------------------------------------------------------------------------------------------------*/
long delay(SYSTEMTIME t1, SYSTEMTIME t2)
{
	long d;

	d = (t2.wSecond - t1.wSecond) * 1000;
	d += (t2.wMilliseconds - t1.wMilliseconds);
	return(d);
}
#include "Client.h"
#define MAXLEN				65000   // Maximum Buffer length
#define DEFLEN				64		// Default Length

void RunClient(Settings* s)
{
	HANDLE ClientThreadHandle;
	DWORD ThreadId;

	int protocol = s->protocol;

	if (protocol == TCP)
	{
		if ((ClientThreadHandle = CreateThread(NULL, 0, TCPClient, (LPVOID)s, 0, &ThreadId)) == NULL)
		{
			OutputDebugString("CreateThread failed with error \n");
			return;
		}
	}

	if (protocol == UDP)
	{
		if ((ClientThreadHandle = CreateThread(NULL, 0, UDPClient, (LPVOID)s, 0, &ThreadId)) == NULL)
		{
			OutputDebugString("CreateThread failed with error \n");
			return;
		}
	}

	return;
}

DWORD WINAPI TCPClient(LPVOID lpParameter)
{
	OutputDebugString("I am TCP Client\n");
	return TRUE;
}

DWORD WINAPI UDPClient(LPVOID lpParameter)
{
	int	data_size = DEFLEN;
	Settings* setting = (Settings*)lpParameter;
	int	i, j, server_len, client_len;
	SOCKET sd;
	char* pname, rbuf[MAXLEN], sbuf[MAXLEN];
	struct	hostent* hp;
	struct	sockaddr_in server, client;
	SYSTEMTIME stStartTime, stEndTime;
	WSADATA stWSAData;
	WORD wVersionRequested = MAKEWORD(2, 2);

	// Initialize the DLL with version Winsock 2.2
	WSAStartup(wVersionRequested, &stWSAData);

	// Create a datagram socket
	if ((sd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("Can't create a socket\n");
		exit(1);
	}

	// Store server's information
	memset((char*)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(setting->port_number);

	if ((hp = gethostbyname(setting->ip_address)) == NULL)
	{
		OutputDebugString("Can't get server's IP address\n");
		return FALSE;
	}
	//strcpy((char *)&server.sin_addr, hp->h_addr);
	memcpy((char*)&server.sin_addr, hp->h_addr, hp->h_length);

	// Bind local address to the socket
	memset((char*)&client, 0, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port = htons(0);  // bind to any available port
	client.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sd, (struct sockaddr*) & client, sizeof(client)) == -1)
	{
		OutputDebugString("Can't bind name to socket\n");
		return FALSE;
	}
	// Find out what port was assigned and print it
	client_len = sizeof(client);
	if (getsockname(sd, (struct sockaddr*) & client, &client_len) < 0)
	{
		OutputDebugString("Getsockname failed!\n");
		return FALSE;
	}
	printf("Port assigned is %d\n", ntohs(client.sin_port));

	data_size = setting->packet_size;

	if (data_size > MAXLEN)
	{
		fprintf(stderr, "Data is too big\n");
		return FALSE;
	}

	// data	is a, b, c, ..., z, a, b,...
	for (i = 0; i < data_size; i++)
	{
		j = (i < 26) ? i : i % 26;
		sbuf[i] = 'a' + j;
	}

	// Get the start time
	GetSystemTime(&stStartTime);

	// transmit data
	server_len = sizeof(server);
	if (sendto(sd, sbuf, data_size, 0, (struct sockaddr*) & server, server_len) == -1)
	{
		perror("sendto failure");
		return FALSE;
	}

	// receive data
	if (recvfrom(sd, rbuf, MAXLEN, 0, (struct sockaddr*) & server, &server_len) < 0)
	{
		perror(" recvfrom error");
		return FALSE;
	}

	//Get the end time and calculate the delay measure
	GetSystemTime(&stEndTime);
	printf("Round-trip delay = %ld ms.\n", delay(stStartTime, stEndTime));

	if (strncmp(sbuf, rbuf, data_size) != 0)
		OutputDebugString("Data is corrupted\n");

	closesocket(sd);
	WSACleanup();

	//SOCKET sock;
	//struct sockaddr_in sin;
	//WSADATA stWSAData;
	//WORD wVersionRequested = MAKEWORD(2, 2);

	//int packet_size;
	//int port;
	//int i;
	//int err;

	//char ip_address[TEXT_BUFFER];
	///*char open_file_path[TEXT_BUFFER];*/
	//char* file_buffer;
	//char* buffer_ptr;

	//OutputDebugString("I am UDP Client\n");

	////Add file contents to buffer here.
	//file_buffer = (char*)malloc(setting->packet_size);

	////Obtain member information from Settings
	//port = setting->port_number;
	//strcpy(ip_address, setting->ip_address);
	///*strcpy(open_file_path, setting->open_file_path);*/
	//
	////ReadFile(open_file_path, &file_buffer);
	////OutputDebugString(file_buffer);

	//for (i = 0; i < setting->packet_size; i++)
	//	file_buffer[i] = 'a';

	//buffer_ptr = file_buffer;

	//// Initialize the DLL with version Winsock 2.2
	//WSAStartup(wVersionRequested, &stWSAData);

	//// Open a connectionless, unreliable socket (Datagrams)
	//if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	//	return FALSE;

	//// Set the socket options such that the send buffer size is set at the
	//// application layer
	//if (err = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, buffer_ptr, sizeof(file_buffer)) != 0)
	//{
	//	OutputDebugString("Error in setsockopt!\n");
	//	return FALSE;
	//}

	//memset(&sin, 0, sizeof(sin));
	//sin.sin_family = AF_INET;	 // Specify the Internet (TCP/IP) Address family
	//sin.sin_port = htons(setting->port_number); // Convert to network byte order

	//// Ensure that the IP string is a legitimate address (dotted decimal)
	//if ((sin.sin_addr.s_addr = inet_addr(setting->ip_address) == INADDR_NONE))
	//{
	//	printf("Invalid IP address\n");
	//	return FALSE;
	//}

	//OutputDebugString("IP Address & Socket Okay\n");

	//// Transmit data through an unconnected (UDP) socket
	//if (sendto(sock, file_buffer, setting->packet_size, 0, (struct sockaddr*) & sin, sizeof(sin)) <= 0)
	//{
	//	perror("sendto error");
	//	return FALSE;
	//}

	//OutputDebugString("Sent a packet!\n");

	//// Close the socket
	//closesocket(sock);

	//// Cleanup after yourself
	//WSACleanup();
	//free(file_buffer);

	return TRUE;
}

// Compute the delay between tl and t2 in milliseconds
long delay(SYSTEMTIME t1, SYSTEMTIME t2)
{
	long d;

	d = (t2.wSecond - t1.wSecond) * 1000;
	d += (t2.wMilliseconds - t1.wMilliseconds);
	return(d);
}
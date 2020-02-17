#include "Client.h"
#define MAXLEN				65000   // Maximum Buffer length
#define SEND_BUFFER			1024
#define TEXT_BUFFER			64		// Default Length

HWND hwnd_clientout;

void RunClient(Settings* s)
{
	HANDLE ClientThreadHandle;
	DWORD ThreadId;

	int protocol = s->protocol;
	hwnd_clientout = s->hwnd_clientout;

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

DWORD WINAPI TCPClient(LPVOID lpParameter)
{
	//Settings* setting = (Settings*)lpParameter;

	//int n, ns, bytes_to_read;
	//int port, err;
	//SOCKET sd;
	//struct hostent* hp;
	//struct sockaddr_in server;
	//TCHAR* host, * bp, rbuf[SEND_BUFFER], ** pptr;
	//TCHAR* sbuf = (TCHAR*)malloc(sizeof(TCHAR) * (SEND_BUFFER + 1));

	//WSADATA WSAData;
	//WORD wVersionRequested;

	//if (!sbuf) {
	//	TCHAR error_msg[] = "Error in malloc file buffer!";
	//	SendOutputMessage(hwnd_clientout, error_msg);
	//	free(sbuf);
	//	return FALSE;
	//}

	//wVersionRequested = MAKEWORD(2, 2);
	//err = WSAStartup(wVersionRequested, &WSAData);
	//if (err != 0) //No usable DLL
	//{
	//	TCHAR error_msg[] = "Error in WSAStartup!";
	//	SendOutputMessage(hwnd_clientout, error_msg);
	//	free(sbuf);
	//	return FALSE;
	//}

	// Create the socket
	//if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	//{
	//	TCHAR error_msg[] = "Error in opening socket!";
	//	SendOutputMessage(hwnd_clientout, error_msg);
	//	free(sbuf);
	//	return FALSE;
	//}

	// Initialize and set up the address structure
	//memset((TCHAR*)&server, 0, sizeof(struct sockaddr_in));
	//server.sin_family = AF_INET;
	//server.sin_port = htons(setting->port_number);
	//if ((hp = gethostbyname(setting->ip_address)) == NULL)
	//{
	//	TCHAR error_msg[] = "Error in calling gethostbyname!";
	//	SendOutputMessage(hwnd_clientout, error_msg);
	//	free(sbuf);
	//	return FALSE;
	//}

	// Copy the server address
	//memcpy((TCHAR*)&server.sin_addr, hp->h_addr, hp->h_length);

	// Connecting to the server
	//if (connect(sd, (struct sockaddr*) & server, sizeof(server)) == -1)
	//{
	//	TCHAR error_msg[] = "Can't connect to server!";
	//	SendOutputMessage(hwnd_clientout, error_msg);
	//	free(sbuf);
	//	return FALSE;
	//}

	//TCHAR serv_name[] = "Connected server name:";
	//TCHAR ip_address[] = "Connected IP address:";

	//SendOutputMessage(hwnd_clientout, serv_name, hp->h_name);
	//pptr = hp->h_addr_list;
	//SendOutputMessage(hwnd_clientout, ip_address, inet_ntoa(server.sin_addr));

	//TCHAR send_msg[] = "Client sent:";
	//SendOutputMessage(hwnd_clientout, send_msg);

	//memset(sbuf, 0, sizeof(sbuf));

	//TCHAR open_file_path[TEXT_BUFFER];
	//strncpy(open_file_path, setting->open_file_path, sizeof(open_file_path));
	//ReadFile(open_file_path, &sbuf);

	//TCHAR send_text[DATA_BUFSIZE];
	//memset(send_text, 0, sizeof(send_text));
	//strcpy(send_text, sbuf);
	//SendOutputMessage(hwnd_clientout, send_text);

	//TCHAR num_bytes_send_msg[] = "Number of bytes sent =";
	//TCHAR num_bytes_sent[TEXT_BUFFER];
	//_itoa(strlen(send_text) + 1, num_bytes_sent, 10);
	//SendOutputMessage(hwnd_clientout, num_bytes_send_msg, num_bytes_sent);

	//SendOutputMessage(hwnd_clientout, sending_msg, sbuf);

	// Transmit data through the socket
	//ns = send(sd, send_text, strlen(send_text) + 1, 0);
	//
	//TCHAR receiving_msg[] = "Client received:";
	//SendOutputMessage(hwnd_clientout, receiving_msg);

	//bp = rbuf;
	//bytes_to_read = SEND_BUFFER;
	//TCHAR r_message[DATA_BUFSIZE];

	// client makes repeated calls to recv until no more data is expected to arrive.
	//while ((n = recv(sd, bp, bytes_to_read, 0)) < SEND_BUFFER)
	//{
	//	memset(r_message, 0, sizeof(r_message));
	//	strcpy(r_message, bp);
	//	SendOutputMessage(hwnd_clientout, r_message);

	//	bp += n;
	//	bytes_to_read -= n;

	//	if (n == 0)
	//		break;
	//}

	//free(sbuf);
	//closesocket(sd);
	//WSACleanup();
	return TRUE;
}

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

	TCHAR* rbuf[MAXLEN];

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
		return FALSE;
	}

	SendOutputMessage(hwnd_clientout, sbuf, setting->packet_size);

	//// Initialize the DLL with version Winsock 2.2
	//err = WSAStartup(wVersionRequested, &stWSAData);
	//if (err != 0) //No usable DLL
	//{
	//	TCHAR error_msg[] = "Error in WSAStartup!";
	//	SendOutputMessage(hwnd_clientout, error_msg);
	//	return FALSE;
	//}

	//// Create a datagram socket
	//if ((sd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
	//{
	//	TCHAR error_msg[] = "Error in opening socket!";
	//	SendOutputMessage(hwnd_clientout, error_msg);
	//	return FALSE;
	//}

	//// Store server's information
	//memset((char*)&server, 0, sizeof(server));
	//server.sin_family = AF_INET;
	//server.sin_port = htons(setting->port_number);

	//if ((hp = gethostbyname(setting->ip_address)) == NULL)
	//{
	//	TCHAR error_msg[] = "Error in calling gethostbyname!";
	//	SendOutputMessage(hwnd_clientout, error_msg);
	//	return FALSE;
	//}
	////strcpy((char *)&server.sin_addr, hp->h_addr);
	//memcpy((char*)&server.sin_addr, hp->h_addr, hp->h_length);

	//// Bind local address to the socket
	//memset((char*)&client, 0, sizeof(client));
	//client.sin_family = AF_INET;
	//client.sin_port = htons(0);  // bind to any available port
	//client.sin_addr.s_addr = htonl(INADDR_ANY);

	//if (bind(sd, (struct sockaddr*) & client, sizeof(client)) == -1)
	//{
	//	TCHAR error_msg[] = "Can't bind name to socket.";
	//	SendOutputMessage(hwnd_clientout, error_msg);
	//	return FALSE;
	//}

	//// Find out what port was assigned and print it
	//client_len = sizeof(client);
	//if (getsockname(sd, (struct sockaddr*) & client, &client_len) < 0)
	//{
	//	TCHAR error_msg[] = "Error in calling getsockname.";
	//	SendOutputMessage(hwnd_clientout, error_msg);
	//	return FALSE;
	//}

	//// Get the start time
	////GetSystemTime(&stStartTime);

	//// transmit data
	//TCHAR send_msg[] = "Client sent:";
	//SendOutputMessage(hwnd_clientout, send_msg);

	//if (sendto(sd, sbuf, packet_size_memory, 0, (struct sockaddr*) & server, sizeof(server)) == -1)
	//{
	//	TCHAR error_msg[] = "Error in calling sendto!";
	//	SendOutputMessage(hwnd_clientout, error_msg);
	//	return FALSE;
	//}

	//TCHAR num_bytes_send_msg[] = "Number of bytes sent =";
	//TCHAR num_bytes_sent[TEXT_BUFFER];
	//_itoa(setting->packet_size, num_bytes_sent, 10);
	//SendOutputMessage(hwnd_clientout, num_bytes_send_msg, num_bytes_sent);

	//// receive data
	//if (recvfrom(sd, rbuf, MAXLEN, 0, (struct sockaddr*) & server, &server_len) < 0)
	//{
	//	TCHAR error_msg[] = "Error in calling recvfrom!";
	//	SendOutputMessage(hwnd_clientout, error_msg);
	//	return FALSE;
	//}

	//Get the end time and calculate the delay measure
	//GetSystemTime(&stEndTime);
	//TCHAR round_trip_delay[TEXT_BUFFER];
	//TCHAR round_trip_delay_msg[] = "Round trip delay in ms =";
	//_itoa(delay(stStartTime, stEndTime), round_trip_delay, 10);
	//SendOutputMessage(hwnd_clientout, round_trip_delay_msg, round_trip_delay);

	//if (strncmp(sbuf, rbuf, data_size) != 0)
	//	OutputDebugString("Data is corrupted\n");

	free(sbuf);
	//closesocket(sd);
	WSACleanup();

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
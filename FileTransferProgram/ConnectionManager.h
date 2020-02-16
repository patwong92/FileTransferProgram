#pragma once
#include "Global.h"
#include "Settings.h"
#include "Widgets.h"

DWORD WINAPI RunServer(LPVOID lpParameter);
DWORD WINAPI TCPWorkerThread(LPVOID lpParameter);
DWORD WINAPI UDPWorkerThread(LPVOID lpParameter);
void CALLBACK TCPWorkerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags);
DWORD CreateSocket(SOCKET* socket, int protocol);
const char* GetInitializationMessage(Settings* s);
SOCKADDR_IN FillAddrStructure(Settings* setting);
DWORD WINAPI WaitingForTCPConnections(LPVOID lpParameter);
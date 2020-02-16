#pragma once
#include "Global.h"

typedef struct {
    int port_number;
    TCHAR ip_address[TEXT_BUFFER];
    int protocol;
    int client_frequency;
    int packet_size;
    TCHAR open_file_path[TEXT_BUFFER];
    TCHAR save_file_path[TEXT_BUFFER];
    HWND hwnd_serverout;
    HWND hwnd_clientout;
} Settings;

void SetDefaultSettings(Settings* s, int port, TCHAR ip_address[], int protocol, int freq, int packet_size, const char* ofile_path, const char* dfile_path);
void ProtocolAsString(int protocol, char* buffer);
int ProtocolAsInt(TCHAR protocol[]);

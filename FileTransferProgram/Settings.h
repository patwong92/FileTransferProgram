#pragma once
#include "Global.h"

typedef struct {
    int port_number;
    TCHAR ip_address[TEXT_BUFFER];
    int protocol;
    int client_frequency;
    int packet_size;
    TCHAR open_file_path[FILE_PATH_BUFFER];
    TCHAR save_file_path[FILE_PATH_BUFFER];
    HWND hwnd_serverout;
    HWND hwnd_clientout;
} Settings;

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: SetDefaultSettings
--
-- DATE: February 11, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: void SetDefaultSettings(Settings* s, int port, TCHAR ip_address[], int protocol, int freq,
            int packet_size, const char* ofile_path, const char* dfile_path);
--          Settings* s - Pointer to the Settings structure
--          int port - Port number
--          TCHAR ip_address - IP address
--          int protocol - 500 is TCP; 501 is UDP
--          int freq - Sending frequency
--          int packet_size - Packet size
--          const char* ofile_path - Open file path
--          const char* dfile_path - Write file path
--
-- RETURNS: void
--
-- NOTES:
-- Sets the default settings to the member functions in the Settings structure.
----------------------------------------------------------------------------------------------------------------------*/
void SetDefaultSettings(Settings* s, int port, TCHAR ip_address[], int protocol, int freq, int packet_size, const char* ofile_path, const char* dfile_path);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ProtocolAsString
--
-- DATE: February 11, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: void ProtocolAsString(int protocol, TCHAR* buffer)
--      int protocol - 500 is TCP; 501 is UDP
--      TCHAR* buffer - Pointer to a TCHAR character. It is a string
--
-- RETURNS: void.
--
-- NOTES:
-- Call this function to convert the integer values of TCP or UDP to "TCP" or "UDP" respectively.
----------------------------------------------------------------------------------------------------------------------*/
void ProtocolAsString(int protocol, TCHAR* buffer);

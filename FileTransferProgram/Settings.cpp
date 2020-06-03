/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: Settings.cpp - Defines the structure to store client and server information
--
-- PROGRAM: FileTransferProgram
--
-- FUNCTIONS:
-- void SetDefaultSettings(Settings* s, int port, TCHAR ip_address[], int protocol, int freq, int packet_size,
        const char* ofile_path, const char* dfile_path)
-- void ProtocolAsString(int protocol, TCHAR* buffer);
--
-- DATE: February 11, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- NOTES:
-- This cpp file defines the Settings structure to store user information for use to run the client or server.
----------------------------------------------------------------------------------------------------------------------*/
#include "Settings.h"

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
void SetDefaultSettings(Settings* s, int port, TCHAR ip_address[], int protocol, int freq, int packet_size, const char* ofile_path, const char* dfile_path)
{
    s->port_number = port;
    strncpy(s->ip_address, ip_address, TEXT_BUFFER);
    s->protocol = protocol;
    s->client_frequency = freq;
    s->packet_size = packet_size;
    strncpy(s->open_file_path, ofile_path, TEXT_BUFFER);
    strncpy(s->save_file_path, dfile_path, TEXT_BUFFER);
}

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
void ProtocolAsString(int protocol, TCHAR* buffer)
{
    const TCHAR* tcp = "TCP";
    const TCHAR* udp = "UDP";
    const TCHAR* unknown = "unknown";
    
    switch (protocol)
    {
    case TCP:
        strncpy(buffer, tcp, sizeof(tcp));
        break;
    case UDP:
        strncpy(buffer, udp, sizeof(udp));
        break;
    default:
        strncpy(buffer, unknown, sizeof(unknown));
        break;
    }
}

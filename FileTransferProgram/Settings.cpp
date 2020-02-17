#include "Settings.h"

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

int ProtocolAsInt(TCHAR protocol[])
{
    if (strstr(protocol, "TCP") != NULL)
        return TCP;

    if (strstr(protocol, "UDP") != NULL)
        return UDP;

    return 0;
}
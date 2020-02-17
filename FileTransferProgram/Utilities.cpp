#include "Utilities.h"

int OpenFileToRead(FILE** read_file, TCHAR* read_path)
{
    if ((*read_file = fopen(read_path, "r")) == NULL)
        return 1;

    return 0;
}

int OpenFileToWrite(FILE** read_file, TCHAR* write_path)
{
    if ((*read_file = fopen(write_path, "w")) == NULL)
        return 1;

    return 0;
}


//Reads less or equal to the packet_size - 1
int ReadFile(TCHAR* read_path, TCHAR** content, int input_packet_size)
{
    FILE* read_file = NULL;
    //TCHAR read_buffer[DATA_BUFSIZE];
    long length;
    long byte_content = input_packet_size - 1;
    long bytes_to_pad;
    int content_size;

    if (OpenFileToRead(&read_file, read_path) != 0)
    {
        OutputDebugString("Error in opening file\n");
        return 1;
    }

    //Seek to end of file to get the length
    if (fseek(read_file, 0L, SEEK_END) != 0)
    {
        OutputDebugString("Error in seeking to end\n");
        fclose(read_file);
        return 1;
    }

    length = ftell(read_file);

    content_size = length > byte_content ? byte_content : length;

    //Seek to beginning to file to write
    if (fseek(read_file, 0L, SEEK_SET) != 0)
    {
        OutputDebugString("Error in seeking to beginning\n");
        fclose(read_file);
        return 1;
    }

    fread(*content, content_size, sizeof(char), read_file);

    //Truncate garbage characters here
    //*content = read_buffer;

    CloseFile(&read_file);

    return 0;
}

int WriteFile(TCHAR* write_path, TCHAR** write_buf)
{
    FILE* write_file = NULL;

    if (OpenFileToWrite(&write_file, write_path) != 0)
    {
        perror("Error in opening file\n");
        return 1;
    }

    if (fseek(write_file, 0L, SEEK_END) != 0)
    {
        printf("Error in seeking!\n");
        fclose(write_file);
        return 1;
    }

    fwrite(*write_buf, 1, strlen(*write_buf) + 1, write_file);
    
    CloseFile(&write_file);

    return 0;
}

int CloseFile(FILE** file)
{
    if (fclose(*file) != 0)
        return 1;

    return 0;
}

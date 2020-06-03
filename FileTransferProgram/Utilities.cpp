/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: Utilites.cpp - Provides file related operations
--
-- PROGRAM: FileTransferProgram
--
-- FUNCTIONS:
-- int OpenFileToRead(FILE** read_file, TCHAR* read_path);
-- int OpenFileToWrite(FILE** read_file, TCHAR* write_path);
-- int ReadFile(TCHAR* read_path, TCHAR** content, int input_packet_size);
-- int WriteFile(TCHAR* write_path, TCHAR** write_buf);
-- int CloseFile(FILE** file);
--
-- DATE: February 11, 2020
--
-- REVISIONS: February 18, 2020; finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- NOTES:
-- This cpp file provides functions that allow the reading and writing of files.
----------------------------------------------------------------------------------------------------------------------*/
#include "Utilities.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: OpenFileToRead
--
-- DATE: February 16, 2020
--
-- REVISIONS: February 18, 2020; finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: int OpenFileToRead(FILE** read_file, TCHAR* read_path)
--		FILE** read_file - Double pointer to the FILE structure
--		TCHAR* read_path - The path containing the file
--
-- RETURNS: 0 if function call is successful
--			1 if function call is unsucessful
--
-- NOTES:
-- Call this function to open the file to read.
----------------------------------------------------------------------------------------------------------------------*/
int OpenFileToRead(FILE** read_file, TCHAR* read_path)
{
    if ((*read_file = fopen(read_path, "r")) == NULL)
        return 1;

    return 0;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: OpenFileToWrite
--
-- DATE: February 16, 2020
--
-- REVISIONS: February 18, 2020; finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: int OpenFileToWrite(FILE** read_file, TCHAR* read_path)
--		FILE** read_file - Double pointer to the FILE structure
--		TCHAR* read_path - The path to write the file
--
-- RETURNS: 0 if function call is successful
--			1 if function call is unsucessful
--
-- NOTES:
-- Call this function to open the file to write
----------------------------------------------------------------------------------------------------------------------*/
int OpenFileToWrite(FILE** read_file, TCHAR* write_path)
{
    if ((*read_file = fopen(write_path, "a")) == NULL)
        return 1;

    return 0;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ReadFile
--
-- DATE: February 16, 2020
--
-- REVISIONS: February 18, 2020; finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: int ReadFile(TCHAR* read_path, TCHAR** content, int input_packet_size)
--			TCHAR* read_path - File path to read the file
--			TCHAR** content - buffer to store file read information
--			int input_packet_size - Packet size of the buffer
--
-- RETURNS: 0 if function call is successful
--			1 if function call is unsucessful
--
-- NOTES:
-- Call this function to open the file to read and store its contents to a buffer.
----------------------------------------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WriteFile
--
-- DATE: February 16, 2020
--
-- REVISIONS: February 18, 2020; finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: int WriteFile(TCHAR* read_path, TCHAR** content, int input_packet_size)
--			TCHAR* write_path - File path to read the file
--			TCHAR** write_buf - buffer that contains data
--
-- RETURNS: 0 if function call is successful
--			1 if function call is unsucessful
--
-- NOTES:
-- Call this function to write contents to a file.
----------------------------------------------------------------------------------------------------------------------*/
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

    TCHAR buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    strcpy(buffer, *write_buf);

    fwrite(*write_buf, 1, strlen(buffer)+1, write_file);
    
    CloseFile(&write_file);

    return 0;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: CloseFile
--
-- DATE: February 16, 2020
--
-- REVISIONS: February 18, 2020; finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: int CloseFile(FILE** file)
--		FILE** file - Double pointer to the FILE structure
--
-- RETURNS: 0 if function call is successful
--			1 if function call is unsucessful
--
-- NOTES:
-- Closes the file for reading and writing.
----------------------------------------------------------------------------------------------------------------------*/
int CloseFile(FILE** file)
{
    if (fclose(*file) != 0)
        return 1;

    return 0;
}

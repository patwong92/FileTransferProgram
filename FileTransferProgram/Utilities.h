#pragma once

#include "Global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 65000

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
int OpenFileToRead(FILE** read_file, TCHAR* read_path);

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
int OpenFileToWrite(FILE** read_file, TCHAR* write_path);

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
int ReadFile(TCHAR* read_path, TCHAR** content, int input_packet_size);

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
int WriteFile(TCHAR* write_path, TCHAR** write_buf);

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
int CloseFile(FILE** file);
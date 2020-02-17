#pragma once

#include "Global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int OpenFileToRead(FILE** read_file, TCHAR* read_path);
int OpenFileToWrite(FILE** read_file, TCHAR* write_path);
int ReadFile(TCHAR* read_path, TCHAR** content, int input_packet_size);
int WriteFile(TCHAR* write_path, TCHAR** write_buf);
int CloseFile(FILE** file);
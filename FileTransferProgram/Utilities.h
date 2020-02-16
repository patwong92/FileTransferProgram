#pragma once

#include "Global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int OpenFileToRead(FILE** read_file, char* read_path);
int OpenFileToWrite(FILE** read_file, char* write_path);
int ReadFile(char* read_path, char** read_buf);
int WriteFile(char* write_path, char** write_buf);
int CloseFile(FILE** file);
void SendOutputMessage(HWND hwnd, char text[]);
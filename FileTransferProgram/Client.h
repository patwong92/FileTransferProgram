#pragma once

#include "Global.h"
#include "Settings.h"
#include "Utilities.h"
#include "Widgets.h"

void RunClient(Settings* s);
DWORD WINAPI TCPClient(LPVOID lpParameter);
DWORD WINAPI UDPClient(LPVOID lpParameter);
long delay(SYSTEMTIME t1, SYSTEMTIME t2);
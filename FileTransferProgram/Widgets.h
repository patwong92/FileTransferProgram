#include "Global.h"

VOID CreateLabel(HWND hwnd, LPCTSTR text, int x, int y);
HWND CreateTextEntry(HWND hwnd, int num, int x, int y, int identifier);
HWND CreateTextEntry(HWND hwnd, TCHAR text[], int x, int y, int identifier);
HWND CreateButton(HWND hwnd, TCHAR text[], int x, int y, int identifier);
void SendOutputMessage(HWND hwnd, TCHAR text[]);
void SendOutputMessage(HWND hwnd, TCHAR text1[], TCHAR text2[]);
void SendOutputMessage(HWND hwnd, TCHAR text[], int size);
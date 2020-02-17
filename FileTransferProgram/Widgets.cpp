#include "Widgets.h"

VOID CreateLabel(HWND hwnd, LPCTSTR text, int x, int y)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	TextOut(hdc, x, y, text, strlen(text) + 1);

	ReleaseDC(hwnd, hdc);
}

HWND CreateTextEntry(HWND hwnd, int num, int x, int y, int identifier)
{
	HWND handle;
	TCHAR length[32];
	sprintf(length, "%d", num);

	if ((handle = CreateWindowExA(0, "EDIT", length, WS_BORDER | WS_VISIBLE | WS_CHILD,
		x, y, 200, 20, hwnd, (HMENU)identifier, GetModuleHandle(NULL), NULL)) == NULL)
		return NULL;
	return handle;
}


HWND CreateTextEntry(HWND hwnd, TCHAR text[], int x, int y, int identifier)
{
	HWND handle;
	if ((handle = CreateWindowExA(0, "EDIT", text, WS_BORDER | WS_VISIBLE | WS_CHILD,
		x, y, 200, 20, hwnd, (HMENU)identifier, GetModuleHandle(NULL), NULL)) == NULL)
			return NULL;
	return handle;
}

HWND CreateButton(HWND hwnd, TCHAR text[], int x, int y, int identifier)
{
	HWND handle;
	if ((handle = CreateWindowExA(0, "BUTTON", text, WS_BORDER | WS_VISIBLE | WS_CHILD,
		x, y, 150, 30, hwnd, (HMENU) identifier, GetModuleHandle(NULL), NULL)) == NULL)
		return NULL;
	return handle;
}

void SendOutputMessage(HWND hwnd, TCHAR text[])
{
	TCHAR message[DATA_BUFSIZE];
	GetWindowText(hwnd, message, DATA_BUFSIZE);
	strcat(message, text);
	strcat(message, "\r\n");
	SetWindowText(hwnd, message);
}

void SendOutputMessage(HWND hwnd, TCHAR text[], int size)
{
	//TCHAR message[DATA_BUFSIZE];

	//GetWindowText(hwnd, message, DATA_BUFSIZE);
	//strcat(message, text);
	//strcat(message, "\r\n");
	//SetWindowText(hwnd, message);

	TCHAR* message = (TCHAR*)malloc(sizeof(TCHAR) * (2*size));

	if (message != NULL)
	{
		memset(message, 0, sizeof(TCHAR) * (2 * size));
		GetWindowText(hwnd, message, sizeof(TCHAR) * (2 * size));
		strncat(message, text, size);
		strcat(message, "\r\n");
		SetWindowText(hwnd, message);
		free(message);
	}
}

//Appends text2 to text1 and outputs the message
void SendOutputMessage(HWND hwnd, TCHAR text1[], TCHAR text2[])
{
	TCHAR message[DATA_BUFSIZE];

	GetWindowText(hwnd, message, DATA_BUFSIZE);
	strcat(message, text1);
	strcat(message, " ");
	strcat(message, text2);
	strcat(message, "\r\n");
	SetWindowText(hwnd, message);
}


//CreateWindowExA(0, "STATIC", "Server Information", WS_VISIBLE | WS_CHILD,
//	20, 20, 200, 30, hwnd, NULL, hInstance, NULL);

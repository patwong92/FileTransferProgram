/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: Widgets.cpp - Provides the interface windows in the application
--
-- PROGRAM: FileTransferProgram
--
-- FUNCTIONS:
-- VOID CreateLabel(HWND hwnd, LPCTSTR text, int x, int y);
-- HWND CreateTextEntry(HWND hwnd, int num, int x, int y, int identifier);
-- HWND CreateTextEntry(HWND hwnd, TCHAR text[], int x, int y, int identifier);
-- HWND CreateButton(HWND hwnd, TCHAR text[], int x, int y, int identifier);
-- void SendOutputMessage(HWND hwnd, TCHAR text[]);
-- void SendOutputMessage(HWND hwnd, TCHAR text1[], TCHAR text2[]);
-- void SendOutputMessage(HWND hwnd, TCHAR text[], int size);
-- void ClearOutputMessage(HWND hwnd);
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
-- This file provides the functions to add GUI objects to the application.
----------------------------------------------------------------------------------------------------------------------*/
#include "Widgets.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: CreateButton
--
-- DATE: February 11, 2020
--
-- REVISIONS: February 18, 2020; finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: HWND CreateButton(HWND hwnd, TCHAR text[], int x, int y, int identifier)
--			HWND hwnd - Window handle
--			TCHAR[] text - Button label
--			int x - X-position on the window
--			int y -	Y-position on the window
--			int identifier - ID of the button
--
-- RETURNS: Handle to the button
--
-- NOTES:
-- Call this function to create a button
----------------------------------------------------------------------------------------------------------------------*/
HWND CreateButton(HWND hwnd, TCHAR text[], int x, int y, int identifier)
{
	HWND handle;
	if ((handle = CreateWindowExA(0, "BUTTON", text, WS_BORDER | WS_VISIBLE | WS_CHILD,
		x, y, 150, 30, hwnd, (HMENU) identifier, GetModuleHandle(NULL), NULL)) == NULL)
		return NULL;
	return handle;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: SendOutputMessage
--
-- DATE: February 11, 2020
--
-- REVISIONS: February 18, 2020; finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: void SendOutputMessage(HWND hwnd, TCHAR text[])
--			HWND hwnd - Window handle
--			TCHAR[] text - text to send
--
-- RETURNS: void
--
-- NOTES:
-- Sends text to the output box.
----------------------------------------------------------------------------------------------------------------------*/
void SendOutputMessage(HWND hwnd, TCHAR text[])
{
	TCHAR message[DATA_BUFSIZE];
	GetWindowText(hwnd, message, DATA_BUFSIZE);
	strcat(message, text);
	strcat(message, "\r\n");
	SetWindowText(hwnd, message);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: SendOutputMessage
--
-- DATE: February 11, 2020
--
-- REVISIONS: February 18, 2020; finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: void SendOutputMessage(HWND hwnd, TCHAR text1[], TCHAR text2[])
--			HWND hwnd - Window handle
--			TCHAR[] text1 - Text to send
--			TCHAR[] text2 - Text to send
--
-- RETURNS: void
--
-- NOTES:
-- Concatenates two texts and sends them to the output box.
----------------------------------------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: SendOutputMessage
--
-- DATE: February 11, 2020
--
-- REVISIONS: February 18, 2020; finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: void SendOutputMessage(HWND hwnd, TCHAR text1[], int size)
--			HWND hwnd - Window handle
--			TCHAR[] text - Text to send
--			int size - Size of the buffer
--
-- RETURNS: void
--
-- NOTES:
-- Given the size of the buffer, the function dynamically allocates memory to send text to thee output box.
----------------------------------------------------------------------------------------------------------------------*/
void SendOutputMessage(HWND hwnd, TCHAR text[], int size)
{
	TCHAR* message = (TCHAR*)malloc(sizeof(TCHAR) * (2 * size));

	if (message != NULL)
	{
		memset(message, 0, sizeof(TCHAR) * (2 * size));
		GetWindowText(hwnd, message, sizeof(TCHAR) * (2 * size));
		strncat(message, text, size);
		strcat(message, "\r\n");
		SetWindowText(hwnd, message);
		free(message);
		message = NULL;
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ClearOutputMessage
--
-- DATE: February 11, 2020
--
-- REVISIONS: February 18, 2020; finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: void ClearOutputMessage(HWND hwnd)
--			HWND hwnd - Window handle
--
-- RETURNS: void
--
-- NOTES:
-- Clears the contents of the output box.
----------------------------------------------------------------------------------------------------------------------*/
void ClearOutputMessage(HWND hwnd)
{
	TCHAR message[DATA_BUFSIZE];
	GetWindowText(hwnd, message, DATA_BUFSIZE);
	memset(message, 0, DATA_BUFSIZE);
	SetWindowText(hwnd, message);
}

#include "Global.h"


//HWND CreateTextEntry(HWND hwnd, int num, int x, int y, int identifier);
//HWND CreateTextEntry(HWND hwnd, TCHAR text[], int x, int y, int identifier);

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
HWND CreateButton(HWND hwnd, TCHAR text[], int x, int y, int identifier);

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
void SendOutputMessage(HWND hwnd, TCHAR text[]);

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
void SendOutputMessage(HWND hwnd, TCHAR text1[], TCHAR text2[]);

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
void SendOutputMessage(HWND hwnd, TCHAR text[], int size);

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
void ClearOutputMessage(HWND hwnd);
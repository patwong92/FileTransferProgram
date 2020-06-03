#pragma once
#include "Global.h"
#include "ConnectionManager.h"
#include "Settings.h"
#include "Client.h"
#include "Widgets.h"
#pragma warning (disable: 4096)

#define DEFAULT_PORT_NUMBER        8000
#define DEFAULT_IP_ADDRESS         "127.0.0.1"
#define DEFAULT_SEND_FREQUENCY     1
#define DEFAULT_PACKET_SIZE        1024
#define DEFAULT_OPEN_FILE_PATH     "./"
#define DEFAULT_SAVE_FILE_PATH     "./textlog.txt"

#define IDC_EDIT_IP_ADDRESS             1000
#define IDC_EDIT_PORT_NUMBER            1001
#define IDC_EDIT_PACKET_SIZE            1002
#define IDC_EDIT_SEND_FREQUENCY         1003
#define IDC_BUTTON_OPEN_FILE            1004
#define IDC_EDIT_OPEN_FILE              1005
#define IDC_RADIO_TCP                   1007
#define IDC_RADIO_UDP                   1008
#define IDC_TEXT_IP_ADDRESS             1011
#define IDC_TEXT_PORT_NUMBER            1012
#define ID_RUN_SETTINGS                 1013
#define IDD_DIALOG1						1014
#define ID_RUN_CLIENT					1015
#define ID_RUN_SERVER					1016

#define ID_CHANGE_SETTINGS_CLIENT		1017
#define ID_CHANGE_SETTINGS_SERVER		1018
#define ID_BUTTON_RUN_SERVER			1019
#define ID_BUTTON_RUN_CLIENT			1020
#define ID_OUTPUT_CLIENT				1021
#define ID_OUTPUT_SERVER				1022

#define IDW_EDIT_SERVER_PROTOCOL		1023
#define IDW_EDIT_SERVER_IP_ADDRESS		1024
#define IDW_EDIT_SERVER_PORT			1025
#define IDW_EDIT_CLIENT_PROTOCOL		1026
#define IDW_EDIT_CLIENT_IP_ADDRESS		1027
#define IDW_EDIT_CLIENT_PORT			1028
#define IDW_EDIT_CLIENT_PACKET_SIZE		1029
#define IDW_EDIT_CLIENT_FREQUENCY		1030

#define IDC_BUTTON_SAVE_RECEIVED_FILES	1031
#define IDC_EDIT_SAVE_FILE_PATH			1032
#define MYMENU							1033
#define ID_HELP							1034
#define IDC_STATIC						1035

#define BUFFER_SIZE						256
#define MARGIN_WINDOW_SIZE				20
#define TEXT_SPACING_SIZE				30

HINSTANCE hInstance;
DWORD ServerThreadId;
LRESULT hNewStaticBrush;
HWND serverhwnd;
HWND clienthwnd;
HWND hwndserveroutput;
HWND hwndclientoutput;
HWND hwnd;

static TCHAR Name[] = TEXT("FileTransferProgram");
static TCHAR ServerWindowName[] = TEXT("Server Window");
static TCHAR ClientWindowName[] = TEXT("Client Window");
Settings* setting = (Settings*)malloc(sizeof(Settings));

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WndProc
--
-- DATE: February 11, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM)
--			HWND hwnd - Window handle
--			UINT message - Windows message
--			WPARAM wparam - Additional message information
--			LPARAM lparam - Additional message information
--
-- RETURNS: 0 if the procedure is successful
--
-- NOTES:
-- Call this function to handle the event system in a Windows application. 
----------------------------------------------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: CreateChildWindow
--
-- DATE: February 11, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: void CreateChildWindow(LPCSTR className, WNDPROC winProcedure, HINSTANCE hInstance)
--			className - Class name of the child window
--			winProcedure - A custom windows procedure function to handle messages
--			hInstance - Handle to the program's executable module 
--
-- RETURNS: void.
--
-- NOTES:
-- Call this function to “daemonize” the application. Basically this function forks a new process,
-- allows the parent process to exit after setting the umask on the child process as global.
----------------------------------------------------------------------------------------------------------------------*/
void CreateChildWindow(LPCSTR className, WNDPROC winProcedure, HINSTANCE hInstance);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ServerWndProc
--
-- DATE: February 11, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: LRESULT CALLBACK ServerWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
--			HWND hwnd - Window handle
--			UINT message - Windows message
--			WPARAM wparam - Additional message information
--			LPARAM lparam - Additional message information
--
-- RETURNS: 0 if the function ends.
--
-- NOTES:
-- Call this function handle window events of the server child window.
----------------------------------------------------------------------------------------------------------------------*/
LRESULT CALLBACK ServerWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ClientWndProc
--
-- DATE: February 11, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: LRESULT CALLBACK ClientWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
--			HWND hwnd - Window handle
--			UINT message - Windows message
--			WPARAM wparam - Additional message information
--			LPARAM lparam - Additional message information
--
-- RETURNS: 0 if the function ends.
--
-- NOTES:
-- Call this function handle window events of the client child window.
----------------------------------------------------------------------------------------------------------------------*/
LRESULT CALLBACK ClientWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: OnCommand
--
-- DATE: February 11, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: void OnCommand(HWND handle, UINT Message, WPARAM wParam, LPARAM lParam)
--			HWND hwnd - Window handle
--			UINT message - Windows message
--			WPARAM wparam - Additional message information
--			LPARAM lparam - Additional message information
--
-- RETURNS: void
--
-- NOTES:
-- Call this function to handle menu messages in the window application. Requires the use of LOWORD(wParam).
----------------------------------------------------------------------------------------------------------------------*/
void OnCommand(HWND handle, UINT Message, WPARAM wParam, LPARAM lParam);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: SettingsDialog
--
-- DATE: February 11, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: BOOL CALLBACK SettingsDialog(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
--			HWND hwnd - Window handle
--			UINT message - Windows message
--			WPARAM wparam - Additional message information
--			LPARAM lparam - Additional message information
--
-- RETURNS: 1 if dialogbox is successful
--			0 if dialogbox is unsuccessful
--
-- NOTES:
-- Handles the buttons, texts and events of the dialog box.
-- The dialog box will be used to update the Settings structure for client and server parameters.
----------------------------------------------------------------------------------------------------------------------*/
BOOL CALLBACK SettingsDialog(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: SetConfiguration
--
-- DATE: February 11, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: void SetConfiguration(HWND hwnd, Settings* setting)
--			HWND hwnd - Window handle
--			Settings* setting - Pointer to the Settings structure to store server and client parameters
--
-- RETURNS: void
--
-- NOTES:
-- Updates user input to the Settings structure.
----------------------------------------------------------------------------------------------------------------------*/
void SetConfiguration(HWND hwnd, Settings* setting);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: OpenFile
--
-- DATE: February 11, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: TCHAR* OpenFile(HWND hwnd)
--			HWND hwnd - Window handle
--
-- RETURNS: ofn.lpstrFile - File path from the Open File dialog box
--
-- NOTES:
-- This function opens the file dialog box for the user to locate the file path to use.
----------------------------------------------------------------------------------------------------------------------*/
TCHAR* OpenFile(HWND hwnd);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: SetDefaultConnectionSettings
--
-- DATE: February 11, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- INTERFACE: void SetDefaultConnectionSettings(void)
--
-- RETURNS: void.
--
-- NOTES:
-- Call this function to set the default values to the Settings structure. The data members are useful in setting up the
-- client or server parameters
----------------------------------------------------------------------------------------------------------------------*/
void SetDefaultConnectionSettings();

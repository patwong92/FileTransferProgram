/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: Main.cpp - An application that either acts as a server to receive file data or as a client to
--							send file data
--
-- PROGRAM: FileTransferProgram
--
-- FUNCTIONS:
-- LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
-- void CreateChildWindow(LPCSTR className, WNDPROC winProcedure, HINSTANCE hInstance)
-- LRESULT CALLBACK ServerWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
-- LRESULT CALLBACK ClientWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
-- void OnCommand(HWND handle, UINT Message, WPARAM wParam, LPARAM lParam)
-- BOOL CALLBACK SettingsDialog(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
-- void SetConfiguration(HWND hwnd, Settings* setting)
-- TCHAR* OpenFile(HWND hwnd)
-- void SetDefaultConnectionSettings()
--
-- DATE: February 11, 2020
--
-- REVISIONS: February 18, 2020, Finalized implementation
--
-- DESIGNER: Patrick Wong
--
-- PROGRAMMER: Patrick Wong
--
-- NOTES:
-- This program is a win32 application that utilizes the winsock2 library to handle TCP and UDP connections. The program
-- can either set up as a TCP/UDP client to send data or a TCP/UDP server to handle data. For the server, the program uses
-- the completion routine model to handle frequent client traffic.
----------------------------------------------------------------------------------------------------------------------*/
#include "Main.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspszCmdParam, int nCmdShow)
{
	MSG Msg;
	WNDCLASSEX Wcl;
	WNDCLASSEX Wclserver;

	// Define a Window class
	Wcl.cbSize = sizeof(WNDCLASSEX);
	Wcl.style = CS_HREDRAW | CS_VREDRAW;  // default style
	Wcl.hIcon = LoadIcon(NULL, (LPCSTR) IDI_APPLICATION); // large icon 
	Wcl.hIconSm = NULL; // use small version of large icon
	Wcl.hCursor = LoadCursor(NULL, IDC_ARROW);  // cursor style

	Wcl.lpfnWndProc = WndProc; // window function
	Wcl.hInstance = hInst; // handle to this instance
	hInst = hInstance;
	Wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //white background
	Wcl.lpszClassName = Name; // window class name
	Wcl.lpszMenuName = MAKEINTRESOURCE(MYMENU); // Class menu
	Wcl.cbClsExtra = 0;      // no extra memory needed
	Wcl.cbWndExtra = 0;

	// Register the class
	if (!RegisterClassEx(&Wcl))
		return 0;

	SetDefaultConnectionSettings();

	hwnd = CreateWindow(
		Name, // name of window class
		Name, // title 
		WS_OVERLAPPEDWINDOW, // window style - normal
		CW_USEDEFAULT,	// X coord
		CW_USEDEFAULT, // Y coord
		CW_USEDEFAULT, // width
		CW_USEDEFAULT, // height
		NULL, // no parent window
		NULL, // no menu
		hInst, // instance handle
		NULL // no additional arguments
	);

	//Create Server Child Window
	CreateChildWindow(ServerWindowName, ServerWndProc, hInstance);
	serverhwnd = CreateWindowExA(0, ServerWindowName, "", WS_VISIBLE | WS_CHILD, 0, 0, 800, 500, hwnd, NULL, hInstance, NULL);
	ShowWindow(serverhwnd, SW_HIDE);

	//Create Client Child Window
	CreateChildWindow(ClientWindowName, ClientWndProc, hInstance);
	clienthwnd = CreateWindowExA(0, ClientWindowName, "", WS_VISIBLE | WS_CHILD, 0, 0, 800, 500, hwnd, NULL, hInstance, NULL);
	ShowWindow(clienthwnd, SW_HIDE);

	// Display the window
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Create the message loop
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg); // translate keybpard messages
		DispatchMessage(&Msg); // dispatch message and return control to windows
	}

	return Msg.wParam;
}

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
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message,
	WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		case WM_COMMAND: {
			OnCommand(hwnd, Message, wParam, lParam);
			break;
		}
		case WM_DESTROY:		// message to terminate the program
			PostQuitMessage(0);
			break;

		default: // Let Win32 process all other messages
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

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
void CreateChildWindow(LPCSTR className, WNDPROC winProcedure, HINSTANCE hInstance)
{
	HWND serverhwmd;
	MSG Msg;
	WNDCLASSEX Wcl;

	// Define a Window class
	Wcl.cbSize = sizeof(WNDCLASSEX);
	Wcl.style = CS_HREDRAW | CS_VREDRAW;  // default style
	Wcl.hIcon = LoadIcon(NULL, (LPCSTR)IDI_APPLICATION); // large icon 
	Wcl.hIconSm = NULL; // use small version of large icon
	Wcl.hCursor = LoadCursor(NULL, IDC_ARROW);  // cursor style
	Wcl.lpfnWndProc = winProcedure; // window function
	Wcl.hInstance = hInstance; // handle to this instance
	Wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //white background
	Wcl.lpszClassName = className; // window class name
	Wcl.lpszMenuName = NULL; // Class menu
	Wcl.cbClsExtra = 0;      // no extra memory needed
	Wcl.cbWndExtra = 0;

	// Register the class
	RegisterClassEx(&Wcl);
}

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
LRESULT CALLBACK ServerWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		case WM_CREATE: {
			TCHAR update[] = "Change Settings";
			TCHAR run_server[] = "Run Server";

			hwndserveroutput = CreateWindowExA(0, "EDIT", NULL, WS_BORDER | WS_VISIBLE | WS_CHILD | WS_VSCROLL| ES_MULTILINE | ES_AUTOVSCROLL, MARGIN_WINDOW_SIZE, 180, 650, 300, hwnd, (HMENU) ID_OUTPUT_SERVER, GetModuleHandle(NULL), NULL);
			setting->hwnd_serverout = hwndserveroutput;

			CreateButton(hwnd, update, 520, 80, ID_CHANGE_SETTINGS_SERVER); //Update
			CreateButton(hwnd, run_server, 520, 130, ID_BUTTON_RUN_SERVER); //Run server
			break;
		}
		case WM_PAINT: {
			TCHAR protocol[10];
			TCHAR title[] = "Server Settings";
			TCHAR param1[] = "Protocol";
			TCHAR param2[] = "IP Address";
			TCHAR param3[] = "Port Number";
			TCHAR output[] = "Output";

			TCHAR port_value[TEXT_BUFFER];
			TCHAR packet_value[TEXT_BUFFER];
			TCHAR freq_value[TEXT_BUFFER];

			ProtocolAsString(setting->protocol, protocol);

			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			TextOut(hdc, MARGIN_WINDOW_SIZE, 20, title, strlen(title) + 1);
			TextOut(hdc, MARGIN_WINDOW_SIZE, 60, param1, strlen(param1) + 1);
			TextOut(hdc, MARGIN_WINDOW_SIZE, 90, param2, strlen(param2) + 1);
			TextOut(hdc, MARGIN_WINDOW_SIZE, 120, param3, strlen(param3) + 1);
			TextOut(hdc, MARGIN_WINDOW_SIZE, 150, output, strlen(output) + 1);
			
			sprintf(port_value, "%d", setting->port_number);
			sprintf(packet_value, "%d", setting->packet_size);
			sprintf(freq_value, "%d", setting->client_frequency);

			TextOut(hdc, 200, 60, protocol, strlen(protocol) + 1);
			TextOut(hdc, 200, 90, setting->ip_address, strlen(setting->ip_address) + 1);
			TextOut(hdc, 200, 120, port_value, strlen(port_value) + 1);

			ReleaseDC(hwnd, hdc);
			break;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case ID_CHANGE_SETTINGS_SERVER: {
					DialogBoxA(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, SettingsDialog);
					ShowWindow(serverhwnd, SW_HIDE);
					ShowWindow(serverhwnd, SW_SHOW);
					break;
				}
				case ID_BUTTON_RUN_SERVER: {

					if ((CreateThread(NULL, 0, RunServer, setting, 0, &ServerThreadId)) == NULL)
					{
						char failmsg[] = "Create server thread failed!";
						SendOutputMessage(hwndserveroutput, failmsg);
					}

					break;
				}
			}
			break;
		}
		case WM_DESTROY:		// message to terminate the program
			PostQuitMessage(0);
			break;

		default: // Let Win32 process all other messages
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

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
LRESULT CALLBACK ClientWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case ID_CHANGE_SETTINGS_CLIENT: {
				DialogBoxA(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, SettingsDialog);
				ShowWindow(clienthwnd, SW_HIDE);
				ShowWindow(clienthwnd, SW_SHOW);
				break;
			}
			case ID_BUTTON_RUN_CLIENT: {
				RunClient(setting);
				break;
			}
			}
			break;
		}
		case WM_CREATE: {
			TCHAR protocol[10];
			TCHAR update[] = "Change Settings";
			TCHAR run_client[] = "Run Client";

			hwndclientoutput = CreateWindowExA(0, "EDIT", NULL, WS_BORDER | WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL, MARGIN_WINDOW_SIZE, 240, 650, 250, hwnd, NULL, GetModuleHandle(NULL), NULL);
			setting->hwnd_clientout = hwndclientoutput;

			CreateButton(hwnd, update, 520, 150, ID_CHANGE_SETTINGS_CLIENT); //Update
			CreateButton(hwnd, run_client, 520, 200, ID_BUTTON_RUN_CLIENT); //Run client
			break;
		}
		case WM_PAINT: {
			TCHAR protocol[10];
			TCHAR title[] = "Client Settings";
			TCHAR param1[] = "Protocol";
			TCHAR param2[] = "IP Address";
			TCHAR param3[] = "Port Number";
			TCHAR param4[] = "Packet size";
			TCHAR param5[] = "Sending frequency";
			TCHAR output[] = "Output";

			TCHAR port_value[TEXT_BUFFER];
			TCHAR packet_value[TEXT_BUFFER];
			TCHAR freq_value[TEXT_BUFFER];

			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			ProtocolAsString(setting->protocol, protocol);

			TextOut(hdc, MARGIN_WINDOW_SIZE, 20, title, strlen(title) + 1);
			TextOut(hdc, MARGIN_WINDOW_SIZE, 60, param1, strlen(param1) + 1);
			TextOut(hdc, MARGIN_WINDOW_SIZE, 90, param2, strlen(param2) + 1);
			TextOut(hdc, MARGIN_WINDOW_SIZE, 120, param3, strlen(param3) + 1);
			TextOut(hdc, MARGIN_WINDOW_SIZE, 150, param4, strlen(param4) + 1);
			TextOut(hdc, MARGIN_WINDOW_SIZE, 180, param5, strlen(param5) + 1);
			TextOut(hdc, MARGIN_WINDOW_SIZE, 210, output, strlen(output) + 1);

			sprintf(port_value, "%d", setting->port_number);
			sprintf(packet_value, "%d", setting->packet_size);
			sprintf(freq_value, "%d", setting->client_frequency);

			TextOut(hdc, 200, 60, protocol, strlen(protocol) + 1);
			TextOut(hdc, 200, 90, setting->ip_address, strlen(setting->ip_address) + 1);
			TextOut(hdc, 200, 120, port_value, strlen(port_value) + 1);
			TextOut(hdc, 200, 150, packet_value, strlen(packet_value) + 1);
			TextOut(hdc, 200, 180, freq_value, strlen(freq_value) + 1);

			ReleaseDC(hwnd, hdc);

			break;
		}
		case WM_DESTROY:		// message to terminate the program
			PostQuitMessage(0);
			break;

		default: // Let Win32 process all other messages
			return DefWindowProc(hwnd, Message, wParam, lParam);
		}
	return 0;
}

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
void OnCommand(HWND handle, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case ID_RUN_SERVER:
	{
		ShowWindow(clienthwnd, SW_HIDE);
		ShowWindow(serverhwnd, SW_SHOW);

		/*RunServer(setting);*/
		break;
	}
	case ID_RUN_CLIENT:
		ShowWindow(serverhwnd, SW_HIDE);
		ShowWindow(clienthwnd, SW_SHOW);
		break;
	case ID_RUN_SETTINGS:
		DialogBoxA(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), handle, SettingsDialog);
		break;
	}
}

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
BOOL CALLBACK SettingsDialog(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_INITDIALOG: {
		SetDlgItemText(hwnd, IDC_EDIT_IP_ADDRESS, setting->ip_address);
		SetDlgItemText(hwnd, IDC_EDIT_OPEN_FILE, setting->open_file_path);
		SetDlgItemText(hwnd, IDC_EDIT_SAVE_FILE_PATH, setting->save_file_path);

		TCHAR port_number_text[BUFFER_SIZE];
		_itoa(setting->port_number, port_number_text, 10);
		SetDlgItemText(hwnd, IDC_EDIT_PORT_NUMBER, port_number_text);

		TCHAR packet_size_text[BUFFER_SIZE];
		_itoa(setting->packet_size, packet_size_text, 10);
		SetDlgItemText(hwnd, IDC_EDIT_PACKET_SIZE, packet_size_text);

		TCHAR frequency_text[BUFFER_SIZE];
		_itoa(setting->client_frequency, frequency_text, 10);
		SetDlgItemText(hwnd, IDC_EDIT_SEND_FREQUENCY, frequency_text);

		if (setting->protocol == TCP)
			CheckDlgButton(hwnd, IDC_RADIO_TCP, BST_CHECKED);

		if (setting->protocol == UDP)
			CheckDlgButton(hwnd, IDC_RADIO_UDP, BST_CHECKED);

		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam))
		{
			case IDC_BUTTON_OPEN_FILE: {
				TCHAR file_path[FILE_PATH_BUFFER];
				TCHAR* ws_file_path = OpenFile(hwnd);
				strncpy(file_path, ws_file_path, sizeof(file_path));
				SetDlgItemText(hwnd, IDC_EDIT_OPEN_FILE, file_path);
				break;
			}
			case IDC_BUTTON_SAVE_RECEIVED_FILES: {
				TCHAR file_path[FILE_PATH_BUFFER];
				TCHAR* ws_file_path = OpenFile(hwnd);
				strncpy(file_path, ws_file_path, sizeof(file_path));
				SetDlgItemText(hwnd, IDC_EDIT_SAVE_FILE_PATH, file_path);
				break;
			}
			case IDOK: {
				SetConfiguration(hwnd, setting);
				EndDialog(hwnd, IDOK);
				break;
			}
			case IDCANCEL:
				EndDialog(hwnd, IDCANCEL);
				break;
			}
			return TRUE;
		}
	}

	return FALSE;
}

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
void SetConfiguration(HWND hwnd, Settings* setting)
{
	TCHAR ip_address[TEXT_BUFFER];
	GetDlgItemText(hwnd, IDC_EDIT_IP_ADDRESS, ip_address, TEXT_BUFFER);
	strncpy(setting->ip_address, ip_address, sizeof(ip_address));

	TCHAR port_number[BUFFER_SIZE];
	GetDlgItemText(hwnd, IDC_EDIT_PORT_NUMBER, port_number, TEXT_BUFFER);
	setting->port_number = atoi(port_number);

	TCHAR packet_size[BUFFER_SIZE];
	GetDlgItemText(hwnd, IDC_EDIT_PACKET_SIZE, packet_size, TEXT_BUFFER);
	setting->packet_size = atoi(packet_size);

	TCHAR frequency[BUFFER_SIZE];
	GetDlgItemText(hwnd, IDC_EDIT_SEND_FREQUENCY, frequency, TEXT_BUFFER);
	setting->client_frequency = atoi(frequency);

	if (IsDlgButtonChecked(hwnd, IDC_RADIO_TCP))
	{
		setting->protocol = TCP;
	}
	else {
		setting->protocol = UDP;
	}

	TCHAR open_file_path[TEXT_BUFFER];
	GetDlgItemText(hwnd, IDC_EDIT_OPEN_FILE, open_file_path, TEXT_BUFFER);
	strncpy(setting->open_file_path, open_file_path, sizeof(open_file_path));

	TCHAR save_file_path[TEXT_BUFFER];
	GetDlgItemText(hwnd, IDC_EDIT_SAVE_FILE_PATH, save_file_path, TEXT_BUFFER);
	strncpy(setting->save_file_path, save_file_path, sizeof(save_file_path));

	UpdateWindow(serverhwnd);
	UpdateWindow(clienthwnd);
}

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
TCHAR* OpenFile(HWND hwnd)
{
	TCHAR file_name[MAX_PATH];

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = file_name;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = "Text Files\0*.txt\0";
	ofn.Flags = OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&ofn)) {
		TCHAR response[FILE_PATH_BUFFER];
		strncpy(response, "./", sizeof(response));
		return response;
	}

	return ofn.lpstrFile;
}

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
void SetDefaultConnectionSettings()
{
	TCHAR default_ip[TEXT_BUFFER] = DEFAULT_IP_ADDRESS;
	TCHAR default_open_file[FILE_PATH_BUFFER] = DEFAULT_OPEN_FILE_PATH;
	TCHAR default_close_file[FILE_PATH_BUFFER] = DEFAULT_SAVE_FILE_PATH;

	SetDefaultSettings(setting,
		DEFAULT_PORT_NUMBER, default_ip, UDP, DEFAULT_SEND_FREQUENCY,
		DEFAULT_PACKET_SIZE, default_open_file, default_close_file);
}

#pragma once
#include "Global.h"
#include "resource.h"
#include "ConnectionManager.h"
#include "Settings.h"
#include "Client.h"
#include "Widgets.h"

#define DEFAULT_PORT_NUMBER        8000
#define DEFAULT_IP_ADDRESS         "127.0.0.1"
#define DEFAULT_SEND_FREQUENCY     10
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


#define BUFFER_SIZE						256
#define MARGIN_WINDOW_SIZE				20
#define TEXT_SPACING_SIZE				30


Settings* setting = (Settings*)malloc(sizeof(Settings));

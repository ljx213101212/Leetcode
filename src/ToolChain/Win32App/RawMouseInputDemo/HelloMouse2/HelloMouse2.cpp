// HelloMouse2.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "HelloMouse2.h"
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <hidsdi.h>

// Identify this as a Windows XP application.  This is necessary(specify the minimum operating system version your code supports. )
#define _WIN32_WINNT 0x0501   
//to pull in RawInput (which is an XP exclusive)
#define WM_INPUT 0x00FF
std::wstring mousemessage_str = L"";
char mousemessage[2048]; // this is the string we draw to the screen for the first mouse 
//char mousemessage2[256]; // this is the string we draw to the screen for the second mouse 
std::wstring mousemessage2 = L"";
char rawinputdevices[256]; // string with number of raw input devices
							  //	so we can access rawinput

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


void InitRawInput() {

	RAWINPUTDEVICE Rid[50]; // allocate storage for 50 device (not going to need this many :) )
	UINT nDevices;
	// used for result code
	INT nResult;

	PRAWINPUTDEVICELIST pRawInputDeviceList;

	if (GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST)) != 0) {
		return;
	}
	pRawInputDeviceList = (RAWINPUTDEVICELIST*)malloc(sizeof(RAWINPUTDEVICELIST) * nDevices);
	GetRawInputDeviceList(pRawInputDeviceList, &nDevices, sizeof(RAWINPUTDEVICELIST));
	// do the job...
	// Get Raw Devices
	for (UINT i = 0; i < nDevices; i++) {

		// how much data do we need to allocate for the device name ?
		UINT nDeviceBufferLength = 0;

		nResult = GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice,
			RIDI_DEVICENAME,
			NULL,
			&nDeviceBufferLength);

		if (nResult < 0) {
			_RPT1(_CRT_WARN, "failed to get raw input device name length %d\n", GetLastError());
			continue;
		}
		//char* wcParsedData = new char[nDeviceBufferLength + 1];
		// allocate memory for device name (wide char)
		WCHAR* wcDeviceName = new WCHAR[nDeviceBufferLength + 1];
		// failed?
		if (wcDeviceName == NULL) {
			delete[] pRawInputDeviceList;
			_RPT1(_CRT_WARN, "failed to allocate memory for raw device names %d\n", GetLastError());
			PostQuitMessage(5);
			return;
		}
		UINT size;
		if (0 == ::GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice, RIDI_PREPARSEDDATA, nullptr, &size)) {
			if (size == 0) { continue; }
			std::vector<uint8_t> buf(size);
			::GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice, RIDI_PREPARSEDDATA, &buf[0], &size);

			PHIDP_PREPARSED_DATA preparsed_data = reinterpret_cast<PHIDP_PREPARSED_DATA>(&buf[0]);
		}

		// get the RAW device name
		nResult = GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice,
			RIDI_DEVICENAME,
			wcDeviceName,
			&nDeviceBufferLength);

		// failed?
		if (nResult < 0) {

			_RPT1(_CRT_WARN, "failed to get actual raw device name %d\n", GetLastError());
			// free the memory for the name
			delete[] wcDeviceName;
			// skip this one
			continue;
		}

		// setup buffer to receive device info
		RID_DEVICE_INFO ridDeviceInfo;
		ridDeviceInfo.cbSize = sizeof(RID_DEVICE_INFO);

		// this tells the api how big the structure is
		nDeviceBufferLength = ridDeviceInfo.cbSize;

		// fetch the device info for this raw device
		nResult = GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice,
			RIDI_DEVICEINFO,
			&ridDeviceInfo,
			&nDeviceBufferLength);

		// failed?
		if (nResult < 0) {

			_RPT1(_CRT_WARN, "didn't get raw input device info %d\n", GetLastError());

			// skip to next device
			continue;

		}
		else

			// is it a keyboard?
			if (ridDeviceInfo.dwType == RIM_TYPEKEYBOARD) {

				_RPT2(_CRT_WARN, "device handle 0x%x (%d)\ndevice name ", pRawInputDeviceList[i].hDevice, i + 1);

				// wchar type
				OutputDebugStringW(wcDeviceName);

				_RPT1(_CRT_WARN, "dwKeyboardMode %d\n", ridDeviceInfo.keyboard.dwKeyboardMode);
				_RPT1(_CRT_WARN, "dwNumberOfFunctionKeys %d\n", ridDeviceInfo.keyboard.dwNumberOfFunctionKeys);
				_RPT1(_CRT_WARN, "dwNumberOfIndicators %d\n", ridDeviceInfo.keyboard.dwNumberOfIndicators);
				_RPT1(_CRT_WARN, "dwNumberOfKeysTotal %d\n", ridDeviceInfo.keyboard.dwNumberOfKeysTotal);
				_RPT1(_CRT_WARN, "dwType %d\n", ridDeviceInfo.keyboard.dwType);
				_RPT1(_CRT_WARN, "dwSubType %d\n", ridDeviceInfo.keyboard.dwSubType);
			}

		// don't need this anymore
		//delete[] wcParsedData;
		delete[] wcDeviceName;

		wcDeviceName = NULL;

	}


	wsprintf((LPWSTR)rawinputdevices, (LPWSTR)"Number of raw input devices: %i\n\n", nDevices);
	// after the job, free the RAWINPUTDEVICELIST
	free(pRawInputDeviceList);
	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x02;
	Rid[0].dwFlags = 0;// RIDEV_NOLEGACY;   // adds HID mouse and also ignores legacy mouse messages
	Rid[0].hwndTarget = NULL;
	// set

	if (RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])) == FALSE) {
		//registration failed. 
		//mousemessage = L"RawInput init failed:\n";
		wsprintf((LPWSTR)mousemessage, (LPWSTR)"RawInput init failed:\n");
	}
	return;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_HELLOMOUSE2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HELLOMOUSE2));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HELLOMOUSE2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_HELLOMOUSE2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   InitRawInput();
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	static HWND   hwndButton = 0;
	static int    cx, cy;          /* Height and width of our button. */
	HDC           hdc;             /* A device context used for drawing */
	PAINTSTRUCT   ps;              /* Also used during window drawing */
	RECT          rc;              /* A rectangle used during drawing */
	LPBYTE lpb;// = new BYTE[dwSize];//LPBYTE lpb = new BYTE[dwSize];
	UINT dwSize;
	RAWINPUT* raw;
	long tmpx, tmpy;
	static long maxx = 0;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rc);
		DrawText(hdc, L"Hello World", strlen("Hello World"), &rc, DT_CENTER);
		
		OffsetRect(&rc, 0, 200); // move the draw position down a bit
		DrawTextA(hdc, (LPCSTR)mousemessage, strlen(mousemessage), &rc, DT_CENTER);
		//DrawText(hdc, (LPWSTR)mousemessage_str.c_str(), mousemessage_str.length(), &rc, DT_CENTER);
		//DrawText(hdc, (LPWSTR)mousemessage2.c_str(), mousemessage2.length(), &rc, DT_CENTER);
		OffsetRect(&rc, 0, 200); // move the draw position down a bit
		//DrawText(hdc, (LPWSTR)mousemessage_str.c_str(), mousemessage_str.length(), &rc, DT_CENTER);
		EndPaint(hWnd, &ps);
		break;
        }
        break;
	case WM_MOUSEMOVE:
	{
		break;
		int xPosAbsolute = GET_X_LPARAM(lParam);
		int yPosAbsolute = GET_Y_LPARAM(lParam);
		mousemessage2 = L"Mouse X: " + std::to_wstring(xPosAbsolute) + L"\n" + L"Mouse Y: " + std::to_wstring(yPosAbsolute) + L"\n";
		// ...
		InvalidateRect(hWnd, 0, TRUE);
		SendMessage(hWnd, WM_PAINT, 0, 0);
		break;
	}
	case WM_INPUT:

	{
		//break;
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
		lpb = (LPBYTE)malloc(sizeof(LPBYTE) * dwSize);
		if (lpb == NULL)
		{
			return 0;
		}
		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize,
			sizeof(RAWINPUTHEADER)) != dwSize)
			OutputDebugString(TEXT("GetRawInputData doesn't return correct size !\n"));

		raw = (RAWINPUT*)lpb;
		if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			tmpx = raw->data.mouse.lLastX;
			tmpy = raw->data.mouse.lLastY;
			maxx = tmpx;

			/* Convert to Directx format */

			//if ((tmpx > 0xff00))

			  //{ 

				//tmpx = -(0xffff - tmpx);

			  //wsprintf(mousemessage, "TMPX WAS HUGE!!!  %ld", tmpx);

			  //  }

			//if (tmpy > 0xff00) tmpy = -(0xffff - tmpy);

			std::wstring customed_mouse_message = std::to_wstring(raw->data.mouse.lLastX) + L"  " + std::to_wstring(raw->data.mouse.lLastY) + L"\n";
			mousemessage_str = customed_mouse_message;
			sprintf_s((LPSTR)mousemessage, 2048, (LPCSTR)"Mouse:hDevice %d \n usFlags=%04x \nulButtons=%04x \nusButtonFlags=%04x \nusButtonData=%04x \nulRawButtons=%04x \nlLastX=%ld \nlLastY=%ld \nulExtraInformation=%04x\r, %ld\n",

				
				raw->header.hDevice,

				raw->data.mouse.usFlags,

				raw->data.mouse.ulButtons,

				raw->data.mouse.usButtonFlags,

				raw->data.mouse.usButtonData,

				raw->data.mouse.ulRawButtons,

				tmpx, //raw->data.mouse.lLastX, 

				tmpy, //raw->data.mouse.lLastY, 

				raw->data.mouse.ulExtraInformation),

				maxx;

		}

		//OutputDebugStringA(mousemessage);
		InvalidateRect(hWnd, 0, TRUE);

		SendMessage(hWnd, WM_PAINT, 0, 0);



		free(lpb);

		break;

	}
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

#include "stdafx.h"
#include "Resource.h"

#include "ParentWindow.h"
#include "ChildWindow.h"

#define WM_RESTOREORIGINALSTYLE (WM_USER + 100)

ParentWindow::ParentWindow(LPWSTR identity)
	: class_name(identity), instance_handle(nullptr)
{
	INITCOMMONCONTROLSEX common_controls;
	common_controls.dwSize = sizeof(INITCOMMONCONTROLSEX);
	common_controls.dwICC = ICC_BAR_CLASSES;
	InitCommonControlsEx(&common_controls);

	int screen_width = GetSystemMetrics(SM_CXFULLSCREEN);
	int screen_height = GetSystemMetrics(SM_CYFULLSCREEN);

	instance_handle = GetModuleHandle(NULL);

	WNDCLASS window_class = { CS_OWNDC, WndProc, 0, 0, instance_handle, (HICON)LoadImage(instance_handle, MAKEINTRESOURCE(IDI_THREADME), IMAGE_ICON, 48, 48, LR_DEFAULTCOLOR), NULL, NULL, MAKEINTRESOURCE(IDC_THREADME), class_name };
	RegisterClass(&window_class);

	//Main window creation
	window_handle = CreateWindowEx(WS_EX_CONTROLPARENT | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE, class_name, L"Main Window", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 100, 100, screen_width - 200, screen_height - 200, NULL, NULL, instance_handle, NULL);

	//StatusBar
	RECT client_rect;
	GetClientRect(window_handle, &client_rect);										//to disable the grip replace it with CCS_BOTTOM
	status_handle = CreateWindowEx(0, STATUSCLASSNAME, L"", WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, client_rect.bottom - client_rect.top - 20, client_rect.right - client_rect.left, 20, window_handle, NULL, instance_handle, NULL);

	//SetCursor(LoadCursor(NULL, IDC_ARROW));

	int status_parts[2] = { client_rect.right - client_rect.left - 170, client_rect.right - client_rect.left };

	SendMessage(status_handle, SB_SETPARTS, 2, (LPARAM)&status_parts);
	SendMessage(status_handle, SB_SETTEXT, 0, (LPARAM)L"Ready");
	SetTimer(window_handle, 1, 1000, NULL);
	SetWindowLongPtr(window_handle, GWLP_USERDATA, (LONG_PTR)this);
	ShowWindow(window_handle, SW_SHOW);
	UpdateWindow(window_handle);
}

ParentWindow::~ParentWindow()
{
	UnregisterClass(class_name, instance_handle);
}

LRESULT WINAPI ParentWindow::WndProc(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam)
{
	ParentWindow *This = (ParentWindow *)GetWindowLongPtr(window_handle, GWLP_USERDATA);
	switch (message) {
		case WM_CREATE:
		{
			ChildWindow ThisOne((LPWSTR)L"BlowMe", window_handle, (LRESULT WINAPI)This->WndProc);
			ChildWindow ThisOne2((LPWSTR)L"BlowMe2", window_handle, (LRESULT WINAPI)This->WndProc);
			ChildWindow ThisOne3((LPWSTR)L"BlowMe3", window_handle, (LRESULT WINAPI)This->WndProc);
			return 0;
		}
		case WM_SYSCOMMAND: //Disable the fucking snap to edge bull shit
		{
			if (wparam == (SC_MOVE | 2)) wparam = SC_SIZE | 9;
			if ((wparam & 0xFFE0) == SC_SIZE && (wparam & 0x000F)) // handles MOVE and SIZE in one "if"
			{
				long int oldStyle = GetWindowLong(window_handle, GWL_STYLE);
				PostMessage(window_handle, WM_RESTOREORIGINALSTYLE, GWL_STYLE, oldStyle);
				SetWindowLong(window_handle, GWL_STYLE, oldStyle & 0xFEFEFFFF); // disable WS_MAXIMIZE and WS_MAXIMIZEBOX
				DefWindowProc(window_handle, WM_SYSCOMMAND, wparam, lparam);
				return 0;
			}
			return DefWindowProc(window_handle, WM_SYSCOMMAND, wparam, lparam);
		}
		case WM_RESTOREORIGINALSTYLE: //Disable the fucking snap to edge bull shit
		{
			if ((long int)wparam == GWL_STYLE)
				SetWindowLong(window_handle, GWL_STYLE, lparam);
			return 0;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(window_handle, &ps);
			// All painting occurs here, between BeginPaint and EndPaint.
			FillRect(hdc, &ps.rcPaint, (HBRUSH)CreateSolidBrush((COLORREF)0x131413U)); //0x131413U);//0x131413L //0xB48246L //0x4F4F2FL
			EndPaint(window_handle, &ps);
			return true;
		}
		//case WM_ERASEBKGND:
		//{
		//	return TRUE;
		//}
		case WM_SIZE:
		{
			SetWindowPos(This->status_handle, nullptr, 0, HIWORD(lparam) - 20, LOWORD(lparam), 20, SWP_NOZORDER);
			int status_parts[2] = { LOWORD(lparam) - 170, LOWORD(lparam) };
			SendMessage(This->status_handle, SB_SETPARTS, 2, (LPARAM)&status_parts);
			InvalidateRect(window_handle, nullptr, TRUE);
			return 0;
		}
		case WM_TIMER:
		{
			wchar_t buffer[30];
			time_t time_now = time(NULL);
			tm value;
			localtime_s(&value, &time_now);
			wcsftime(buffer, sizeof(buffer), L"%a %d %b %Y %H:%M:%S", &value);
			SendMessage(This->status_handle, SB_SETTEXT, 1, (LPARAM)buffer);
			return 0;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(window_handle, message, wparam, lparam);
}

void ParentWindow::RunWindow()
{
	MSG window_message;
	while (GetMessage(&window_message, nullptr, 0, 0)) {
		TranslateMessage(&window_message);
		DispatchMessage(&window_message);
	}
}
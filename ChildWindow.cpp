#include "stdafx.h"
#include "Resource.h"

#include "ChildWindow.h"

#define WM_RESTOREORIGINALSTYLE (WM_USER + 100)


ChildWindow::ChildWindow(LPWSTR identity, HWND parent, LRESULT WINAPI ParentProc)
	: class_name(identity), parent_window(parent), instance_handle(nullptr)
{
	INITCOMMONCONTROLSEX common_controls;
	common_controls.dwSize = sizeof(INITCOMMONCONTROLSEX);
	common_controls.dwICC = ICC_BAR_CLASSES;
	InitCommonControlsEx(&common_controls);

	instance_handle = GetModuleHandle(NULL);

	//WNDCLASS window_class = { CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, instance_handle, (HICON)LoadImage(instance_handle, MAKEINTRESOURCE(IDI_THREADME), IMAGE_ICON, 48, 48, LR_DEFAULTCOLOR), nullptr, nullptr, MAKEINTRESOURCE(IDC_THREADME), class_name };
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;// CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)MdiWndProc;// ParentProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instance_handle; //(HICON)LoadImage(instance_handle, MAKEINTRESOURCE(IDI_THREADME), IMAGE_ICON, 48, 48, LR_DEFAULTCOLOR)
	wc.hIcon = LoadIcon(instance_handle, MAKEINTRESOURCE(IDI_THREADME));
	wc.hIconSm = LoadIcon(instance_handle, MAKEINTRESOURCE(IDI_THREADME));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_3DSHADOW + 1);
	wc.lpszMenuName = nullptr;// MAKEINTRESOURCE(IDC_THREADME);
	wc.lpszClassName = class_name;

	RegisterClassEx(&wc);

	//Main window creation													//WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX WS_CHILD //WS_THICKFRAME //WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW
	//window_handle = CreateWindowEx(WS_EX_MDICHILD, class_name, L"ChildWindow", (WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW), CW_USEDEFAULT, CW_USEDEFAULT, 220, 110, parent_window, nullptr, instance_handle, NULL);
	window_handle = CreateWindowEx(
		WS_EX_CLIENTEDGE,//WS_EX_CLIENTEDGE
		class_name,
		L"WINDOW_ID",
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,//WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		600,
		400,
		parent_window,
		NULL,
		instance_handle,
		NULL);

	SetWindowLongPtr(window_handle, GWLP_USERDATA, (LONG_PTR)this);
	ShowWindow(window_handle, SW_SHOW);
	UpdateWindow(window_handle);

}


ChildWindow::~ChildWindow()
{
	UnregisterClass(class_name, instance_handle);
}

LRESULT WINAPI ChildWindow::MdiWndProc(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam)
{
	ChildWindow *This = (ChildWindow *)GetWindowLongPtr(window_handle, GWLP_USERDATA);
	switch (message) {
	/*case WM_SYSCOMMAND: //Disable the fucking snap to edge bull shit (Shouldent be needed in the MDI Process)
	{
		if (wparam == (SC_MOVE | 2)) wparam = SC_SIZE | 9;
		if ((wparam & 0xFFE0) == SC_SIZE && (wparam & 0x000F)) // handles MOVE and SIZE in one "if"
		{
			long int oldStyle = GetWindowLong(window_handle, GWL_STYLE);
			PostMessage(window_handle, WM_RESTOREORIGINALSTYLE, GWL_STYLE, oldStyle);
			SetWindowLong(window_handle, GWL_STYLE, oldStyle & 0xFEFEFFFF); // disable WS_MAXIMIZE and WS_MAXIMIZEBOX
			DefMDIChildProc(window_handle, WM_SYSCOMMAND, wparam, lparam);
			return 0;
		}
		return DefMDIChildProc(window_handle, WM_SYSCOMMAND, wparam, lparam);
	}
	case WM_RESTOREORIGINALSTYLE: //Disable the fucking snap to edge bull shit
	{
		if ((long int)wparam == GWL_STYLE)
			SetWindowLong(window_handle, GWL_STYLE, lparam);
		return 0;
	}*/
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
		//SetWindowPos(This->status_handle, nullptr, 0, HIWORD(lparam) - 20, LOWORD(lparam), 20, SWP_NOZORDER);
		//int status_parts[2] = { LOWORD(lparam) - 170, LOWORD(lparam) };
		//SendMessage(This->status_handle, SB_SETPARTS, 2, (LPARAM)&status_parts);
		//InvalidateRect(window_handle, nullptr, TRUE);
		return 0;
	}
	case WM_TIMER:
	{
		//wchar_t buffer[30];
		//time_t time_now = time(NULL);
		//tm value;
		//localtime_s(&value, &time_now);
		//wcsftime(buffer, sizeof(buffer), L"%a %d %b %Y %H:%M:%S", &value);
		//SendMessage(This->status_handle, SB_SETTEXT, 1, (LPARAM)buffer);
		return 0;
	}
	case WM_MDICREATE:
	{
		OutputDebugString(L"WM_MDICREATE\r\n");
		return 0;
	}
	case WM_CREATE:
	{
		OutputDebugString(L"WM_CREATE\r\n");
		return 0;
	}
	case WM_CLOSE:
	{
		//PostQuitMessage(0);
		DestroyWindow(window_handle); //will destroy this window handle WM_DESTROY 
		return 0;
	}
	}
	return DefMDIChildProc(window_handle, message, wparam, lparam);
}

/*void ChildWindow::RunWindow()
{
	MSG window_message;
	while (GetMessage(&window_message, nullptr, 0, 0)) {
		if (!TranslateMDISysAccel(window_handle, &window_message))
		{
			TranslateMessage(&window_message);
			DispatchMessage(&window_message);
		}
	}
}*/

HWND ChildWindow::GetWindow()
{
	return window_handle;
}
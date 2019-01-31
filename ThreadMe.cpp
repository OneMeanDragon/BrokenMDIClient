// ThreadMe.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ThreadMe.h"

#include "ParentWindow.h"


#define TMR_STATUS			70002		//GLOBALS
#define TMR_STATUS_INTERVAL	2500 //2.5seconds
void SetStatusTimer(HWND);



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	ParentWindow MainWindow((LPWSTR)L"my base window");
	MainWindow.RunWindow();

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

void SetStatusTimer(HWND hWnd)
{
	SetTimer(hWnd, TMR_STATUS, TMR_STATUS_INTERVAL, (TIMERPROC)NULL);
}

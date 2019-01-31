#pragma once
class ChildWindow
{
private:
	HWND parent_window;
	LPWSTR class_name;
	HINSTANCE instance_handle;
	HWND window_handle;
	HWND status_handle;
	RECT client_rectangle;
	HCURSOR cursor_arrow;
public:
	ChildWindow(LPWSTR strIdentity, HWND hParent, LRESULT WINAPI ParentProc); //Identity BotName
	~ChildWindow();

	static LRESULT WINAPI MdiWndProc(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam);

	HWND GetWindow();
};


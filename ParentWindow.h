#pragma once
class ParentWindow
{
private:
	LPWSTR class_name;
	HINSTANCE instance_handle;
	HWND window_handle;
	HWND status_handle;
	RECT client_rectangle;
	HCURSOR cursor_arrow;

	HANDLE WorkerThread;
public:
	ParentWindow(LPWSTR identity);
	~ParentWindow();

	static LRESULT WINAPI WndProc(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam);
	static DWORD WINAPI OpenMDI(LPWSTR param);

	void RunWindow();
};


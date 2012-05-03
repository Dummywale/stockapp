// WinStockApp.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WinStockApp.h"



#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK	AddDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WINSTOCKAPP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINSTOCKAPP));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
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
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINSTOCKAPP));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WINSTOCKAPP);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_STOCK_ADD:
			DialogBox(hInst,MAKEINTRESOURCE(IDADD),hWnd,AddDlgProc);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *data)
{
	const char* from = (const char*) ptr;
	char *to = (char*) malloc((nmemb*size) - 59);
	int to_be_read = (int)(nmemb*size) - 43;
	strcpy(to, "");
	strncpy(to, from+42, to_be_read);
	strcat(to, "\0");
	FILE *writehere = (FILE *)data;
	struct json_object *new_obj;
	new_obj = json_tokener_parse((char*)ptr);
	new_obj = json_object_object_get(new_obj, "glossary");
	return fwrite(ptr, size, nmemb, writehere);
}

//Message Handler for dialogbox
BOOL CALLBACK  AddDlgProc (HWND hAddDlg,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
	case WM_COMMAND : 
		if(LOWORD(wParam) == IDADDBTN)
		{
			CURL *curl;
			CURLcode res;
			curl = curl_easy_init();
			FILE *ftpfile;
			LPWSTR  szBuf;
			szBuf = (LPWSTR)GlobalAlloc(GMEM_FIXED,sizeof(LPWSTR) *100);
			LPWSTR szHost;
			szHost = (LPWSTR)GlobalAlloc(GMEM_FIXED,sizeof(LPWSTR) * 100); 

			int jk = GetDlgItemText(hAddDlg,IDC_EDIT1, szBuf, 100);
			StrCpyW(szHost,L"");
			StrCatW(szHost, L"http://finance.yahoo.com/aq/autoc?query=");
			StrCatW(szHost, szBuf);
			StrCatW(szHost,L"&region=US&lang=en-US&callback=YAHOO.util.ScriptNodeDataSource.callbacks");

			char *ansistr = (char*)malloc(sizeof(char)*100);
			WideCharToMultiByte(CP_ACP, 0, (LPWSTR)szHost, -1,ansistr, 1000, NULL, NULL);

			if(curl)
			{
				struct curl_slist *chunk = NULL;
				//"http://finance.yahoo.com/aq/autoc?query=tata&region=US&lang=en-US&callback=YAHOO.util.ScriptNodeDataSource.callbacks"
				chunk = curl_slist_append(chunk, "Host: d.yimg.com");
				chunk = curl_slist_append(chunk, "Accept-Language: en-US,en;q=0.8");
				chunk = curl_slist_append(chunk, "Accept-Encoding: gzip,deflate,sdch");
				chunk = curl_slist_append(chunk, "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
				curl_easy_setopt(curl,CURLOPT_URL,ansistr);
				// curl_easy_setopt(curl,CURLOPT_URL,"http://finance.yahoo.com/aq/autoc?query=tata&region=US&lang=en-US&callback=YAHOO.util.ScriptNodeDataSource.callbacks");
				//            GlobalFree(szHost);
				curl_easy_setopt (curl, CURLOPT_REFERER, "http://finance.yahoo.com/"); 
				curl_easy_setopt (curl, CURLOPT_USERAGENT, "Mozilla 2003, that coolish version"); 
				curl_easy_setopt (curl, CURLOPT_HTTPHEADER,chunk);
				ftpfile = fopen("JSON-resp.txt", "wb");
				curl_easy_setopt(curl, CURLOPT_WRITEDATA,ftpfile); 
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data); 
				res = curl_easy_perform(curl);
				fclose(ftpfile);
				curl_easy_cleanup(curl);

			}         

		}
		if(LOWORD(wParam) == IDCANCEL)
		{
			MessageBox(NULL,_T("Cancel Add"),_T("Cancel Stock"),IDYES);
			EndDialog(hAddDlg,TRUE);
		}
	}
	return FALSE;
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

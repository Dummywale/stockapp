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
     FILE *writehere = (FILE *)data;
  return fwrite(ptr, size, nmemb, writehere);

}
//Message Handler for dialogbox
BOOL CALLBACK  AddDlgProc (HWND hAddDlg,UINT message,WPARAM wParam,LPARAM lParam)
{
    
   /*DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    BOOL  bResults = FALSE;
    HINTERNET  hSession = NULL, 
             hConnect = NULL,
             hRequest = NULL;*/
    switch(message)
    {
    case WM_COMMAND : 
        if(LOWORD(wParam) == IDADDBTN)
        {
          CURL *curl;
          CURLcode res;
          curl = curl_easy_init();
          FILE *ftpfile;
          FILE *respfile;
          LPWSTR  szBuf;
          szBuf = (LPWSTR)GlobalAlloc(GMEM_FIXED,sizeof(LPWSTR));
          LPWSTR szHost;
          szHost = (LPWSTR)GlobalAlloc(GMEM_FIXED,sizeof(LPWSTR)); 
         //szHost = L"http://finance.yahoo.com/aq/autoc?query=";
          //szBuf = NULL;
         int jk = GetDlgItemText(hAddDlg,IDC_EDIT1, szBuf, sizeof(szBuf));
        
        
       
              StrCpyW(szHost,L"");
              StrCatW(szHost, L"http://finance.yahoo.com/aq/autoc?query=");
              StrCatW(szHost, szBuf);
              GlobalFree(szBuf);
              StrCatW(szHost,L"&region=US&lang=en-US&callback=YAHOO.util.ScriptNodeDataSource.callbacks");
     
          if(curl)
          {
             struct curl_slist *chunk = NULL;
            //"http://finance.yahoo.com/aq/autoc?query=tata&region=US&lang=en-US&callback=YAHOO.util.ScriptNodeDataSource.callbacks"
            chunk = curl_slist_append(chunk, "Host: d.yimg.com");
            chunk = curl_slist_append(chunk, "Accept-Language: en-US,en;q=0.8");
            chunk = curl_slist_append(chunk, "Accept-Encoding: gzip,deflate,sdch");
            chunk = curl_slist_append(chunk, "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
            curl_easy_setopt(curl,CURLOPT_URL,szHost);
            GlobalFree(szHost);
            curl_easy_setopt (curl, CURLOPT_REFERER, "http://finance.yahoo.com/"); 
            curl_easy_setopt (curl, CURLOPT_USERAGENT, "Mozilla 2003, that coolish version"); 
            curl_easy_setopt (curl, CURLOPT_HTTPHEADER,chunk);
            ftpfile = fopen("ftp-list.txt", "wb");
            curl_easy_setopt(curl, CURLOPT_WRITEDATA,ftpfile); 
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data); 
            res = curl_easy_perform(curl);
            fclose(ftpfile);
            curl_easy_cleanup(curl);
          }
          /*HINTERNET hpage;
 
           // MessageBox(NULL,_T("Yo it ll ADD"),_T("Add Stock"),IDYES);
            // Use WinHttpOpen to obtain a session handle.
          hSession = WinHttpOpen( L"WinHTTP Example/1.0",  
                                    WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                    WINHTTP_NO_PROXY_NAME, 
                                    WINHTTP_NO_PROXY_BYPASS, 0 );
            // Specify an HTTP server.
            LPCTSTR lpszServerName;
            lpszServerName = (LPCTSTR)"www.google.com";
            INTERNET_PORT nServerPort = INTERNET_DEFAULT_HTTP_PORT;
            if( hSession )
              hConnect = WinHttpConnect( hSession, lpszServerName,
                                         nServerPort, 0 );

            // Create an HTTP request handle.
            if( hConnect )
              hRequest = WinHttpOpenRequest( hConnect, L"GET", L"/",
                                             NULL, WINHTTP_NO_REFERER, 
                                             WINHTTP_DEFAULT_ACCEPT_TYPES, 
                                             WINHTTP_FLAG_SECURE );                        
                                    
            // Send a request.
              if( hRequest )
                bResults = WinHttpSendRequest( hRequest,
                                               WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                                               WINHTTP_NO_REQUEST_DATA, 0, 
                                               0, 0 );


              // End the request.
              if( bResults )
                bResults = WinHttpReceiveResponse( hRequest, NULL );

              // Keep checking for data until there is nothing left.
              if( bResults )
              {
                do 
                {
                  // Check for available data.
                  dwSize = 0;
                  if( !WinHttpQueryDataAvailable( hRequest, &dwSize ) )
                    printf( "Error %u in WinHttpQueryDataAvailable.\n",
                            GetLastError( ) );

                  // Allocate space for the buffer.
                  pszOutBuffer = new char[dwSize+1];
                  if( !pszOutBuffer )
                  {
                    printf( "Out of memory\n" );
                    dwSize=0;
                  }
                  else
                  {
                    // Read the data.
                    ZeroMemory( pszOutBuffer, dwSize+1 );

                    if( !WinHttpReadData( hRequest, (LPVOID)pszOutBuffer, 
                                          dwSize, &dwDownloaded ) )
                      printf( "Error %u in WinHttpReadData.\n", GetLastError( ) );
                    else
                      printf( "%s", pszOutBuffer );

                    // Free the memory allocated to the buffer.
                    delete [] pszOutBuffer;
                  }
                } while( dwSize > 0 );
              }


              // Report any errors.
              if( !bResults )
               { printf( "Error %d has occurred.\n", GetLastError( ) );
               DWORD i = GetLastError();
                char szTest[10];
                sprintf_s(szTest, "%d", GetLastError());
                MessageBox(NULL,LPCWSTR(szTest),_T("Add Stock"),IDYES);
                
                }

              // Close any open handles.
              if( hRequest ) WinHttpCloseHandle( hRequest );
              if( hConnect ) WinHttpCloseHandle( hConnect );
              if( hSession ) WinHttpCloseHandle( hSession );*/
            
          //  EndDialog(hAddDlg,TRUE);
          
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

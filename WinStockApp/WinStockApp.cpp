// WinStockApp.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WinStockApp.h"



#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
char stockDelims []= "~";
struct StocksData
{
    char *szName;
    char *szCode;
    float fPrice; 
    char *szData;
};
// = (StocksData *) malloc(100 * sizeof(StocksData )); 
StocksData *stkData[100];
int count = 0;
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
static size_t getStockValue(void *ptr, size_t size, size_t nmemb, void *data)
{
    StocksData *strData = (StocksData *)data;
    strData[count].szData = (char *)ptr;
	return size * nmemb;

}
void getStockInfo()
{
    CURL *curl;
    struct curl_slist *chunk = NULL;
    CURLcode res;
    curl = curl_easy_init();
    char *szHostName = (char*)malloc(sizeof(char)*100);
    strcpy(szHostName,"");
    strcpy(szHostName,"http://download.finance.yahoo.com/d/quotes.csv?s=");;
    //char szStock = *stock;  
    strcat(szHostName,stkData[count]->szCode);
    strcat(szHostName,"&f=sl1d1t1c1ohgv&e=.csv");
    //"http://finance.yahoo.com/aq/autoc?query=tata&region=US&lang=en-US&callback=YAHOO.util.ScriptNodeDataSource.callbacks"
    chunk = curl_slist_append(chunk, "Host: download.finance.yahoo.com");
    chunk = curl_slist_append(chunk, "Accept-Language: en-US,en;q=0.8");
    chunk = curl_slist_append(chunk, "Accept-Encoding: gzip,deflate,sdch");
    chunk = curl_slist_append(chunk, "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
    curl_easy_setopt(curl,CURLOPT_URL,szHostName);
    curl_easy_setopt (curl, CURLOPT_USERAGENT, "Mozilla 2003, that coolish version"); 
    curl_easy_setopt (curl, CURLOPT_HTTPHEADER,chunk);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,&getStockValue);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,stkData[count]); 
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
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
	static int  cxChar, cxCaps, cyChar ;
	HDC hdc;
	TCHAR       szBuffer [10] ;
    TEXTMETRIC  tm ;
    FILE *pWatchList; 
    pWatchList = fopen("stocks.txt","rb");
    long lSize;
    char * buffer;
    size_t result;
	switch (message)
	{
	case WM_CREATE:
        
        hdc = GetDC (hWnd) ;
        GetTextMetrics (hdc, &tm) ;
        cxChar = tm.tmAveCharWidth ;
        cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2 ;
        cyChar = tm.tmHeight + tm.tmExternalLeading ;
        if (pWatchList!=NULL) 
        {
            fseek (pWatchList , 0 , SEEK_END);
            lSize = ftell (pWatchList);
            rewind (pWatchList);
            // allocate memory to contain the whole file:
            buffer = (char*) malloc (sizeof(char)*(lSize-1));
            // copy the file into the buffer:
            result = fread (buffer,1,lSize-1,pWatchList);
            fclose (pWatchList);
            char *stock = NULL;
            char *stockDetails = NULL;
            stock = strtok( buffer, stockDelims );
            int k = 0;

            while( stock != NULL ) 
            {
                printf( "result is \"%s\"\n", stock );
                stkData[count] = (StocksData *)malloc(sizeof(StocksData));
                stkData[count]->szName = stock;
                stock = strtok( NULL, stockDelims );
                stkData[count]->szCode = stock;
                getStockInfo();
                stock = strtok( NULL, stockDelims );
                count++;

                //stockDetails =strtok(stockData[count].szData,delims);
                // stockData[count].szNmae = stockDetails;
                // stockData[count].szCode = strtok(NULL,delims);


            }

        } 
          break;
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
        //MessageBox(NULL,_T("Cancel Add"),_T("Cancel Stock"),IDYES);
        for (int i = 0 ; i < count; i++)
        {
            
            
            TextOut (hdc, 0, cyChar * i,(LPWSTR)stkData[i]->szName,sizeof(stkData[i]->szName)) ;

            TextOut (hdc, 22 * cxCaps, cyChar * i,      
                 (LPWSTR)stkData[i]->szCode,
                sizeof(stkData[i]->szCode)) ;

            //SetTextAlign (hdc, TA_RIGHT | TA_TOP) ;

            /*TextOut (hdc, 22 * cxCaps + 40 * cxChar, cyChar * i,
                (LPWSTR)stkData[i]->szData,
                 sizeof(stkData[i]->szName)) ;*/

           // SetTextAlign (hdc, TA_LEFT | TA_TOP) ;
        }
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
	FILE *stockNicks = fopen("stocks.txt","ab");
	array_list* results;
	const char* from = (const char*) ptr;
	struct json_object *new_obj;
	struct json_object *name_obj, *nick_obj;

	char *to = (char*) malloc((nmemb*size));
	int to_be_read = (int)(nmemb*size) - 43;

	strcpy(to, "");
	strncpy(to, from+42, to_be_read);
	to[(nmemb*size)-43] = '\0';
	
	new_obj = json_tokener_parse(to);
	new_obj = json_object_object_get(new_obj, "ResultSet");
	new_obj = json_object_object_get(new_obj, "Result");
	results = json_object_get_array(new_obj);
	new_obj = (struct json_object *)array_list_get_idx(results, 0);
	nick_obj = json_object_object_get(new_obj, "symbol");
	name_obj = json_object_object_get(new_obj, "name");
	if (stockNicks != NULL)
	{
	    fprintf(stockNicks,"%s~%s~",json_object_to_json_string(name_obj),json_object_to_json_string(nick_obj));
	    
	    return size*nmemb;
	}
	fclose(stockNicks);	
	return size*nmemb;
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
			FILE *ftpfile = fopen("JSON-resp.txt", "wb");;
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
				
				chunk = curl_slist_append(chunk, "Host: d.yimg.com");
				chunk = curl_slist_append(chunk, "Accept-Language: en-US,en;q=0.8");
				chunk = curl_slist_append(chunk, "Accept-Encoding: gzip,deflate,sdch");
				chunk = curl_slist_append(chunk, "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
				curl_easy_setopt(curl,CURLOPT_URL,ansistr);
		
				curl_easy_setopt (curl, CURLOPT_REFERER, "http://finance.yahoo.com/"); 
				curl_easy_setopt (curl, CURLOPT_USERAGENT, "Mozilla 2003, that coolish version"); 
				curl_easy_setopt (curl, CURLOPT_HTTPHEADER,chunk);				
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, ftpfile); 
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data); 
				res = curl_easy_perform(curl);
				curl_easy_cleanup(curl);
			}
			
			fclose(ftpfile);
			HWND hParentHandle;
			//hParentHandle = GetParent(hAddDlg);
			//InvalidateRect(hParentHandle,NULL,false);
			EndDialog(hAddDlg,TRUE);

		}
		if(LOWORD(wParam) == IDCANCEL)
		{
			//MessageBox(NULL,_T("Cancel Add"),_T("Cancel Stock"),IDYES);
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

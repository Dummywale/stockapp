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
    char *szPrice; 
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
  /* StocksData *strData= (StocksData *)data;
    strData->szData = (char *)ptr;*/
    stkData[count]->szData = (char *)ptr;
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
    res = curl_easy_perform(curl);
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
	 wchar_t *lpszStockRate;
	 wchar_t *lpszStockName;
	 wchar_t *lpszStockCode;
	 FILE *pWatchList;
	PAINTSTRUCT ps;
	static int  cxChar, cxCaps, cyChar ;
	HDC hdc;
	TCHAR       szBuffer [10] ;
    TEXTMETRIC  tm ;
   
    int lSize = 0;
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
        pWatchList = fopen("stocks.txt","r");
        if (pWatchList!=NULL) 
        {
            //fseek (pWatchList , 0 ,SEEK_END);
            
            char data[10000] = {'\0'};
            int counter = 0;
            while((data[counter++] = getc(pWatchList)) != EOF) {
                fprintf(stdout, "%c", data[counter-1]);
                lSize++;
            }
            //lSize = ftell (pWatchList);
            //printf("%ld", lSize); 
            rewind (pWatchList);
            // allocate memory to contain the whole file:
            if(lSize> 0)
            {
                buffer = (char*) malloc (sizeof(char)*(lSize-1));
                strcpy(buffer, "");
                // copy the file into the buffer:
                result = fread (buffer,1,lSize-1,pWatchList);
                buffer[lSize-1] = '\0';
                fclose (pWatchList);
                char *stock = NULL;
                char *stockDetails = NULL;
                stock = strtok( buffer, stockDelims );
                while( stock != NULL ) 
                {
                    stkData[count] = (StocksData *)malloc(sizeof(StocksData));
                    stkData[count]->szName = stock;
                    stock = strtok( NULL, stockDelims );
                    stkData[count]->szCode = stock;
                    getStockInfo();
                    stock = strtok( NULL, stockDelims );
                    count++;
                }
            }
           
            for (int x=0; x<count;x++)
            {
                char *price = NULL;
                price = strtok(stkData[x]->szData,",");
                price = strtok(NULL,",");
                stkData[x]->szPrice = price;

            }
            
            // curl_easy_cleanup(curl);

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
             lpszStockName = (wchar_t *)malloc(sizeof(wchar_t)* 100);
            lpszStockCode = (wchar_t *)malloc(sizeof(wchar_t)* 10);
            lpszStockRate = (wchar_t *)malloc(sizeof(wchar_t)* 10);
            StrCpyW(lpszStockName, L"");
             StrCpyW(lpszStockCode, L"");
              StrCpyW(lpszStockRate, L"");
        for (int n = 0 ; n < count; n++)
        {
            MultiByteToWideChar(CP_ACP, 0, stkData[n]->szName, -1, lpszStockName, strlen(stkData[n]->szName));
            TextOut(hdc,0,cyChar * n,lpszStockName,strlen(stkData[n]->szName));
            MultiByteToWideChar(CP_ACP, 0, stkData[n]->szCode, -1, lpszStockCode, strlen(stkData[n]->szCode));
            TextOut(hdc, 40* cxCaps,cyChar * n,lpszStockCode,strlen(stkData[n]->szCode));
            MultiByteToWideChar(CP_ACP, 0, stkData[n]->szPrice, -1, lpszStockRate, strlen(stkData[n]->szPrice));
            TextOut(hdc, 75* cxCaps,cyChar * n,lpszStockRate,strlen(stkData[n]->szPrice));
        }
      //  free(lpszStockName);
      //   free(lpszStockCode);
        //  free(lpszStockRate);
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
	
	//FILE *writehere = fopen("stocks.txt","a");
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
	//constant char * name = ;
	//cons char *code = ;
	
	 stkData[count] = (StocksData *)malloc(sizeof(StocksData));
	stkData[count]->szCode = (char*)json_object_to_json_string(nick_obj);
	stkData[count]->szName = (char*)json_object_to_json_string(name_obj) ;
	getStockInfo();
	char *price = NULL;
	price =strtok(stkData[count]->szData,",");
	price =strtok(NULL,",");
	stkData[count]->szPrice = price;
	count++;
	if (writehere != NULL)
	{
	    fprintf(writehere,"%s~%s~",json_object_to_json_string(name_obj),json_object_to_json_string(nick_obj));
	    //fclose(writehere);
	}
   // fclose(writehere);
   
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
		
			LPWSTR  szBuf;
			szBuf = (LPWSTR)GlobalAlloc(GMEM_FIXED,sizeof(LPWSTR) *100);
			LPWSTR szHost;
			szHost = (LPWSTR)GlobalAlloc(GMEM_FIXED,sizeof(LPWSTR) * 100); 
			int jk = GetDlgItemText(hAddDlg,IDC_EDIT1, szBuf, 100);
			StrCpyW(szHost,L"");
			StrCatW(szHost, L"http://finance.yahoo.com/aq/autoc?query=");
			StrCatW(szHost, szBuf);
			StrCatW(szHost,L"&region=US&lang=en-US&callback=YAHOO.util.ScriptNodeDataSource.callbacks");
			
			char *getCodeUrl = (char*)malloc(sizeof(char)*200);
			WideCharToMultiByte(CP_ACP, 0, (LPWSTR)szHost, -1,getCodeUrl, 1000, NULL, NULL);

			if(curl)
			{
				struct curl_slist *chunk = NULL;
				FILE *stockFile =  fopen("stocks.txt", "a");
				chunk = curl_slist_append(chunk, "Host: d.yimg.com");
				chunk = curl_slist_append(chunk, "Accept-Language: en-US,en;q=0.8");
				chunk = curl_slist_append(chunk, "Accept-Encoding: gzip,deflate,sdch");
				chunk = curl_slist_append(chunk, "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3");
				curl_easy_setopt(curl,CURLOPT_URL,getCodeUrl);
		
				curl_easy_setopt (curl, CURLOPT_REFERER, "http://finance.yahoo.com/"); 
				curl_easy_setopt (curl, CURLOPT_USERAGENT, "Mozilla 2003, that coolish version"); 
				curl_easy_setopt (curl, CURLOPT_HTTPHEADER,chunk);				
				curl_easy_setopt(curl, CURLOPT_WRITEDATA,stockFile); 
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data); 
				res = curl_easy_perform(curl);
				curl_easy_cleanup(curl);
				free(getCodeUrl);
				fclose(stockFile);
			}
			
			
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

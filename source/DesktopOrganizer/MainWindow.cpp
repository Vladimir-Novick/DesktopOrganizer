// MainWindow.cpp : Implementation of CMainWindow

#include "stdafx.h"
#include "MainWindow.h"
#include ".\mainwindow.h"
#include "resource.h"
#include "RegisterSave.h"
#include "MsgExitWindow.h"
#include "CursorToolBox.h"

    static HHOOK hhkGetMessage = NULL;
    static HHOOK hhkCallWndProc = NULL;
    static HMODULE  hmodHook;

   typedef struct _ITEMMENU
    { 
        WORD Whfont;  // HFONT 
        char psz_ItemText[255]; 
	    WORD WhBitmap;
    } ITEMMENU;             // structure for item font and string  


    CCursorToolBox *m_CursorToolBox;
	HWND m_MainWindowHWND;

    static CMainWindow *m_MainWindow;
	static ITEMMENU ItemAbout;
	static ITEMMENU ItemExit;
	static ITEMMENU ItemShowToolbox;
	static ITEMMENU ItemConfig;
	static ITEMMENU ItemCursor;


	DWORD WINAPI 	ThreadCursorToolbar(LPVOID pData){

		m_CursorToolBox = new CCursorToolBox;

					m_CursorToolBox->DoModal(m_MainWindowHWND);
					delete m_CursorToolBox;
					m_CursorToolBox = NULL;

    return 0;

	}


LRESULT CMainWindow::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<CMainWindow>::OnInitDialog(uMsg, wParam, lParam, bHandled);
		HICON hIcon = LoadIcon(m_InstanceApp, MAKEINTRESOURCE(IDI_ICONSERVER_TOOLS_WINDOW));
		SetIcon(hIcon,TRUE);


DWORD dwDisposition;
HKEY hkeyRun2;

       RegCreateKeyEx(HKEY_CURRENT_USER,_T("Software\\Software by Vladimir Novick\\Desktop Organaizer"),
        0,
        "",
        REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS,
        NULL, //Security
		&hkeyRun2,&dwDisposition);
        RegCloseKey(hkeyRun2); 

DWORD dwVersion = GetVersion();
 

if (dwVersion >= 0x80000000)                // 95 /98
{
       RegCreateKeyEx(HKEY_CURRENT_USER,_T("Environment"),
        0,
        "",
        REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS,
        NULL, //Security
		&hkeyRun2,&dwDisposition);
        RegCloseKey(hkeyRun2); 
}

		SetWindowText("Desktop Organaizer");

        m_CursorToolBox = NULL;
        m_MainWindowHWND = this->m_hWnd;
 

  NOTIFYICONDATA nid;
   nid.cbSize = sizeof(nid);
   nid.hWnd = this->m_hWnd;        
   nid.uID = IDI_ICONSERVER_TOOLS_WINDOW;   
   nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
   nid.uCallbackMessage = UM_DISPRESICONNOTIFY;
   nid.hIcon = LoadIcon(m_InstanceApp, MAKEINTRESOURCE(IDI_ICONSERVER_TOOLS_WINDOW));
   lstrcpyn(nid.szTip, _T("Desktop Organaizer"), sizeof(nid.szTip) / sizeof(nid.szTip[0]));
   Shell_NotifyIcon(NIM_ADD, &nid);

   SendMessage(this->m_hWnd, WM_SETICON, (WPARAM) TRUE, (LPARAM) nid.hIcon);

   ::SetWindowLong(this->m_hWnd, GWL_EXSTYLE,
	    ::GetWindowLong(this->m_hWnd, GWL_EXSTYLE)  | WS_EX_TOOLWINDOW );

   ModifyStyleEx(WS_EX_APPWINDOW,0);


m_TollBox = new CToolBox;
m_TollBox->Create(this->m_hWnd);
m_TollBox->ShowWindow(SW_SHOW);
   ShowWindow(SW_HIDE);


AppTrackStart();

					    DWORD dwA;

						HANDLE hThreadB = ::CreateThread(NULL, NULL, ThreadCursorToolbar, NULL, NULL, 
                                   &dwA);


		return 0;  // Let the system set the focus
	}


// CMainWindow




LRESULT CMainWindow::OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{


		return 0;
}

	LRESULT CMainWindow::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{

		return 0;
	}



	LRESULT CMainWindow::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
   NOTIFYICONDATA nid;
	nid.cbSize = sizeof(nid);
	nid.hWnd = this->m_hWnd ;
	nid.uID = IDI_ICONSERVER_TOOLS_WINDOW;
	Shell_NotifyIcon(NIM_DELETE, &nid);
	AppTrackExit();
	if (m_CursorToolBox != NULL ){
	  m_CursorToolBox->EndDialog(0);
	}
		return 0;
	}






DWORD WINAPI ThreadMainExit(LPVOID pData)
{
 						CMsgExitWindow *m_MsgExutWindow;
						m_MsgExutWindow = new CMsgExitWindow;
						if (m_MsgExutWindow->DoModal(GetDesktopWindow())== 1 ){  
					       m_MainWindow->EndDialog(0);
						} else {
						  m_MainWindow = NULL;
						}

    return 0;
}


	CMainWindow::CMainWindow()
	{
		m_MainWindow = NULL;
		hwndToolBar = NULL;
	}





	LRESULT CMainWindow::OnTryMessage(UINT BuMsg , WPARAM wParam, LPARAM uMsg, BOOL& bHandled)//OnTryMessage(HWND hwnd, UINT uID, UINT uMsg) 
	{

   DWORD dwModeNum = (DWORD) -1; // # of DEVMODEs
   DWORD dwMenuIndex = 0;  // Index into menu where item should be inserted
   HMENU hmenuMain = NULL;
   HMENU hmenuContext = NULL;


   HMENU ghMenu = NULL;
   BOOL startVal=FALSE;


  switch (uMsg) {
	  
     case WM_LBUTTONUP:  // The menu was cancelled
		  {
			m_TollBox->ShowWindow(SW_SHOW); 
		  }
		  break;
 
      case WM_RBUTTONUP:
		  {
		   hmenuMain = LoadMenu(m_InstanceApp, 
		      MAKEINTRESOURCE(IDR_RBUTTON));
		   hmenuContext = GetSubMenu(hmenuMain, 0);
		   CreateMenuListRBotton(hmenuMain,hmenuContext);
		  break;
		  }
      default:
          return ERROR_SUCCESS;
  }
		      // Display the context menu near the mouse cursor
			POINT pt;
		   GetCursorPos(&pt);
			  UINT uMenuID = TrackPopupMenu(hmenuContext, 
		      TPM_BOTTOMALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD,
		      pt.x, pt.y, 0, this->m_hWnd, NULL);



			DestroyMenu(hmenuContext);
			DestroyMenu(hmenuMain);
   

			::PostMessage(this->m_hWnd , WM_NULL, 0, 0);	

			switch (uMenuID) {
				case 0:  // The menu was cancelled
				break;


				case ID_EXIT:
					{


						DWORD dwA;
						m_MainWindow = this;

						HANDLE hThreadB = CreateThread(NULL, NULL, ThreadMainExit, NULL, NULL, 
                                   &dwA);

		
					}
				break;

				case ID_SHOWCURSORTOOLBOX:
				{
					if (m_CursorToolBox== NULL ){
						DWORD dwA;

						HANDLE hThreadB = ::CreateThread(NULL, NULL, ThreadCursorToolbar, NULL, NULL, 
                                   &dwA);
					} else {
						m_CursorToolBox->EndDialog(0);
					}

					break;
				}

				case ID_SHOWTOOLBOX:
				{
					DWORD w_style = m_TollBox->GetWindowLong(GWL_STYLE);

				if (WS_VISIBLE & w_style ){
					m_TollBox->ShowWindow(SW_HIDE);
					this->ShowWindow(SW_HIDE);
				} else {
					m_TollBox->ShowWindow(SW_SHOW);
					this->ShowWindow(SW_HIDE);
				}
					break;
				}

  //   default:
  //  	::PostMessage(this->m_hWnd , WM_NULL, 0, 0);	      
				}

          
/*         RECT rc;
         GetWindowRect(hwnd, &rc);
         SetWindowPos(hwnd, NULL, 
            (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) / 2,
            (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) / 2,
            0, 0, SWP_NOZORDER | SWP_NOSIZE);
*/
 

   return ERROR_SUCCESS;
	}


void CMainWindow::CreateMenuListRBotton(HMENU hmenuMain,HMENU hmenuContext){


    DWORD w_style = m_TollBox->GetWindowLong(GWL_STYLE);

	if (WS_VISIBLE & w_style ){
	  ItemShowToolbox.WhBitmap = IDB_HIDE_TOOLBOX;
	  strcpy(ItemShowToolbox.psz_ItemText," Hide Toolbox");
	} else {

	  ItemShowToolbox.WhBitmap = IDB_SHOW_TOOLBOX;
	  strcpy(ItemShowToolbox.psz_ItemText," Show Toolbox");

	}

   ModifyMenu(hmenuContext, ID_SHOWTOOLBOX, MF_BYCOMMAND | 
                MF_OWNERDRAW, ID_SHOWTOOLBOX, (LPTSTR) &ItemShowToolbox); 

	
	ItemCursor.WhBitmap = IDB_CURSOR_TOOLBAR_PLUS;
		  if (m_CursorToolBox == NULL ){
	strcpy(ItemCursor.psz_ItemText," Start Cursor DragBox");
		  } else {
	strcpy(ItemCursor.psz_ItemText," Stop Cursor DragBox");
		  }

   ModifyMenu(hmenuContext, ID_SHOWCURSORTOOLBOX, MF_BYCOMMAND | 
                MF_OWNERDRAW, ID_SHOWCURSORTOOLBOX, (LPTSTR) &ItemCursor); 


	ItemAbout.WhBitmap = IDB_ABOUT;
	strcpy(ItemAbout.psz_ItemText," About Desktop Organaizer");

   ModifyMenu(hmenuContext, ID_ABOUT, MF_BYCOMMAND | 
                MF_OWNERDRAW, ID_ABOUT, (LPTSTR) &ItemAbout); 

	ItemConfig.WhBitmap = IDB_CONFIG;
	strcpy(ItemConfig.psz_ItemText," Configuration");

   ModifyMenu(hmenuContext, ID_CONFIG, MF_BYCOMMAND | 
                MF_OWNERDRAW, ID_CONFIG, (LPTSTR) &ItemConfig); 



	ItemExit.WhBitmap = IDB_CLOSE;
	strcpy(ItemExit.psz_ItemText,"Shut down Desktop Organaizer");



    ModifyMenu(hmenuContext, ID_EXIT, MF_BYCOMMAND | 
                MF_OWNERDRAW, ID_EXIT, (LPTSTR) &ItemExit); 


   
}
void CMainWindow::DrawTransparentBitmap(HDC hdc, HBITMAP hBitmap, long xStart,
                           long yStart, COLORREF cTransparentColor)
   {
   BITMAP     bm;
   COLORREF   cColor;
   HBITMAP    bmAndBack, bmAndObject, bmAndMem, bmSave;
   HBITMAP    bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
   HDC        hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
   POINT      ptSize;

   hdcTemp = CreateCompatibleDC(hdc);
   SelectObject(hdcTemp, hBitmap);   // Select the bitmap

   ::GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);
   ptSize.x = bm.bmWidth;            // Get width of bitmap
   ptSize.y = bm.bmHeight;           // Get height of bitmap
   DPtoLP(hdcTemp, &ptSize, 1);      // Convert from device

                                     // to logical points

   // Create some DCs to hold temporary data.
   hdcBack   = CreateCompatibleDC(hdc);
   hdcObject = CreateCompatibleDC(hdc);
   hdcMem    = CreateCompatibleDC(hdc);
   hdcSave   = CreateCompatibleDC(hdc);

   // Create a bitmap for each DC. DCs are required for a number of
   // GDI functions.

   // Monochrome DC
   bmAndBack   = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   // Monochrome DC
   bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   bmAndMem    = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
   bmSave      = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);

   // Each DC must select a bitmap object to store pixel data.
   bmBackOld   = (HBITMAP )SelectObject(hdcBack, bmAndBack);
   bmObjectOld = (HBITMAP )SelectObject(hdcObject, bmAndObject);
   bmMemOld    = (HBITMAP )SelectObject(hdcMem, bmAndMem);
   bmSaveOld   = (HBITMAP )SelectObject(hdcSave, bmSave);

   // Set proper mapping mode.
   SetMapMode(hdcTemp, GetMapMode(hdc));

   // Save the bitmap sent here, because it will be overwritten.
   BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

   // Set the background color of the source DC to the color.
   // contained in the parts of the bitmap that should be transparent
   cColor = SetBkColor(hdcTemp, cTransparentColor);

   // Create the object mask for the bitmap by performing a BitBlt
   // from the source bitmap to a monochrome bitmap.
   BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0,
          SRCCOPY);

   // Set the background color of the source DC back to the original
   // color.
   SetBkColor(hdcTemp, cColor);

   // Create the inverse of the object mask.
   BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0,
          NOTSRCCOPY);

   // Copy the background of the main DC to the destination.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, xStart, yStart,
          SRCCOPY);

   // Mask out the places where the bitmap will be placed.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);

   // Mask out the transparent colored pixels on the bitmap.
   BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);

   // XOR the bitmap with the background on the destination DC.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);

   // Copy the destination to the screen.
   BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y, hdcMem, 0, 0,
          SRCCOPY);

   // Place the original bitmap back into the bitmap sent here.
   BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);

   // Delete the memory bitmaps.
   DeleteObject(SelectObject(hdcBack, bmBackOld));
   DeleteObject(SelectObject(hdcObject, bmObjectOld));
   DeleteObject(SelectObject(hdcMem, bmMemOld));
   DeleteObject(SelectObject(hdcSave, bmSaveOld));

   // Delete the memory DCs.
   DeleteDC(hdcMem);
   DeleteDC(hdcBack);
   DeleteDC(hdcObject);
   DeleteDC(hdcSave);
   DeleteDC(hdcTemp);
   } 
	LRESULT CMainWindow::ShowMenuItem_WM_DRAWITEM (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)//(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 

    ITEMMENU *pmyitem;      // pointer to item's font and string        
//    static HMENU hmenu;             // handle to main menu            
    static COLORREF crSelText;  // text color of selected item        
    static COLORREF crSelBkgnd; // background color of selected item  
    COLORREF crText;            // text color of unselected item      
    COLORREF crBkgnd;           // background color unselected item   
          
    LPDRAWITEMSTRUCT lpdis;     // pointer to item drawing data        
    HDC hdc;                    // handle to screen DC                
    SIZE size;                  // menu-item text extents             
    WORD wCheckX;               // check-mark width                   
    int nTextX;                 // width of menu item                 
    int nTextY;                 // height of menu item 
    HFONT hfontOld;             // handle to old font                 
    BOOL fSelected = FALSE;     // menu-item selection flag      
    BITMAP  bm;
	HBITMAP hBmp;
	HFONT hFont;
	DWORD ItemHeight;


	hdc = ::GetWindowDC(menu_hwnd); 

			


		    crSelText = RGB(15,54,85); 
		    crSelBkgnd = RGB(155,182,200); 


 
            lpdis = (LPDRAWITEMSTRUCT) lParam; 
            pmyitem = (ITEMMENU *) lpdis->itemData; 



			         hBmp = LoadBitmap(m_InstanceApp,MAKEINTRESOURCE(pmyitem->WhBitmap));

		    GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bm);
			
 
            if (lpdis->itemState & ODS_SELECTED) 
            { 
                crText = SetTextColor(lpdis->hDC, crSelText); 
                crBkgnd = SetBkColor(lpdis->hDC, crSelBkgnd); 
                fSelected = TRUE; 
            } 
 
            wCheckX = GetSystemMetrics(SM_CXMENUCHECK); 

            wCheckX = 0; 
            nTextX = wCheckX ;
			nTextX = nTextX + lpdis->rcItem.left+bm.bmWidth; 
            nTextY = lpdis->rcItem.top; 
 
			if ( pmyitem->Whfont == 1 ) {
 			  hFont = GetAFontA(0);
              hfontOld = (HFONT)SelectObject(lpdis->hDC,hFont ); 
			}

			 

			
           GetTextExtentPoint32(hdc, pmyitem->psz_ItemText, 
                lstrlen(pmyitem->psz_ItemText), &size); 
			if (size.cy < bm.bmHeight ){
				nTextY = nTextY + (bm.bmHeight-size.cy)/2 ;
			}			
		
			ItemHeight = bm.bmHeight;
			
		ExtTextOut(lpdis->hDC, nTextX, nTextY, ETO_OPAQUE, 
                &lpdis->rcItem, pmyitem->psz_ItemText, 
                lstrlen(pmyitem->psz_ItemText), NULL); 

//			TextOut(lpdis->hDC, nTextX, nTextY, 
//               pmyitem->psz_ItemText, 
 //               lstrlen(pmyitem->psz_ItemText));



			if ( pmyitem->Whfont == 1 ) {
            SelectObject(lpdis->hDC, hfontOld); 
            }
 
            if (fSelected) 
            { 
               SetTextColor(lpdis->hDC, crText); 
               SetBkColor(lpdis->hDC, crBkgnd); 
            } 

			POINT pS;
            nTextY = lpdis->rcItem.top; 
			pS.x = wCheckX/2 + lpdis->rcItem.left;
			pS.x = lpdis->rcItem.left;
			pS.y = nTextY;

            DrawTransparentBitmap(lpdis->hDC,hBmp,pS.x,pS.y,RGB(0,128,0));
			DeleteObject(hBmp);



			if ( pmyitem->Whfont == 1 ) {
			if (fSelected){
			    hBmp = LoadBitmap(m_InstanceApp,MAKEINTRESOURCE(IDB_SELECTED));
		        GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bm);
            pS.y = nTextY+(ItemHeight - bm.bmHeight) / 2;
                DrawTransparentBitmap(lpdis->hDC,hBmp,pS.x,pS.y,RGB(0,128,0));
			    DeleteObject(hBmp);
			} else {
			    hBmp = LoadBitmap(m_InstanceApp,MAKEINTRESOURCE(IDB_SELECTING));
		        GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bm);
                pS.y = nTextY+(ItemHeight - bm.bmHeight) / 2;
              DrawTransparentBitmap(lpdis->hDC,hBmp,pS.x,pS.y,RGB(0,128,0));
			    DeleteObject(hBmp);
			}
			  DeleteObject(hFont);
			}

			::ReleaseDC(menu_hwnd, hdc); 
   return ERROR_SUCCESS;
} 




////--------------

///////////////////////////////////////////////////////////////////////////////

LRESULT CMainWindow::ShowMenuItem_WM_MEASUREITEM(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)//(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 


    ITEMMENU *pmyitem;      // pointer to item's font and string        
//    static HMENU hmenu;             // handle to main menu            

   

    LPMEASUREITEMSTRUCT lpmis;  // pointer to item of data             
      
    HDC hdc;                    // handle to screen DC                
    SIZE size;                  // menu-item text extents             
                
              
 
    HFONT hfontOld;             // handle to old font                 
    BOOL fSelected = FALSE;     // menu-item selection flag      
    BITMAP  bm;
	HBITMAP hBmp;
	HFONT hFont;



	hdc = ::GetWindowDC(menu_hwnd); 
            lpmis = (LPMEASUREITEMSTRUCT) lParam; 
            pmyitem = (ITEMMENU *) lpmis->itemData; 

			hBmp = LoadBitmap(m_InstanceApp,MAKEINTRESOURCE(pmyitem->WhBitmap));
		    GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bm);
			DeleteObject(hBmp);

			hFont = GetAFontA(0);
            hfontOld = (HFONT)SelectObject(hdc,hFont ); 
 
            GetTextExtentPoint32(hdc, pmyitem->psz_ItemText, 
                lstrlen(pmyitem->psz_ItemText), &size); 
            lpmis->itemWidth = size.cx + bm.bmWidth ;
			if (size.cy < bm.bmHeight ){
				lpmis->itemHeight = bm.bmHeight ;
			}else {
            lpmis->itemHeight = size.cy; 
			}
 
 
            SelectObject(hdc, hfontOld); 
			::ReleaseDC(menu_hwnd, hdc); 
			DeleteObject(hFont);
 return ERROR_SUCCESS;
} 

HFONT CMainWindow::GetAFontA(int fnFont) 
{ 
    static LOGFONT lf;  // structure for font information  
 
 
    GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), 
        &lf); 
 
 
    if (fnFont == 1) lf.lfWeight = FW_BOLD; 
    
 
    return CreateFont(lf.lfHeight, lf.lfWidth, 
        lf.lfEscapement, lf.lfOrientation, lf.lfWeight, 
        lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet, 
        lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality, 
        lf.lfPitchAndFamily, lf.lfFaceName); 

}




LRESULT CMainWindow::OnShowWindow(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 0;
}



BOOL CMainWindow::AppTrackStart(void)
{
    if (!hmodHook)
        {
            if (!(hmodHook = LoadLibrary("trackdll.dll")))
            {
               return FALSE;
            }
        }

	  typedef LRESULT  (CALLBACK* InstallHookType)(HWND);

 
	InstallHookType InstallHookPtr = NULL;
	InstallHookPtr = (InstallHookType)GetProcAddress(hmodHook, "InstallHook");
  
    if (InstallHookPtr != NULL ){
        InstallHookPtr(this->m_hWnd);
	}

     InstallHookPtr = NULL;

return true;
}

void CMainWindow::AppTrackExit(void)
{
	if (hmodHook){
	  typedef LRESULT  (CALLBACK* UninstallHookType)(VOID);

 
	UninstallHookType UninstallHookPtr = NULL;
    UninstallHookPtr = (UninstallHookType)GetProcAddress(hmodHook, "UninstallHook");
  
    if (UninstallHookPtr != NULL ){
        UninstallHookPtr();
	}


UninstallHookPtr = NULL;
FreeLibrary(hmodHook);
	}

}

LRESULT CMainWindow::OnCopyData(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
						if (((PCOPYDATASTRUCT)lParam)->dwData == WM_LBUTTONDOWN ) {
							if ( m_CursorToolBox != NULL ){
		                       m_CursorToolBox->ShowWindow(SW_SHOWNOACTIVATE);
							}
						}

						if (((PCOPYDATASTRUCT)lParam)->dwData == WM_LBUTTONUP ) {
							if ( m_CursorToolBox != NULL ){
								POINT pt;
								GetCursorPos(&pt);
								HWND thWnd = WindowFromPoint(pt);
 
								if (thWnd != m_CursorToolBox->m_hWnd){ 
								     m_CursorToolBox->ShowWindow(SW_HIDE);
								}
							}
						}

	return 0;
}

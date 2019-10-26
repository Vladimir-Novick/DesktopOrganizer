// ToolBox.cpp : Implementation of CToolBox

#include "stdafx.h"
#include "ToolBox.h"
#include ".\toolbox.h"
#include "RegisterSave.h"

static HWND tools_hWnd;
static  BOOL boolStartTrackingShow;
static CToolBox *gbl_Window;
static BOOL StartTrack3;
static BOOL bWindowFixed;

// CToolBox


LRESULT CToolBox::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<CToolBox>::OnInitDialog(uMsg, wParam, lParam, bHandled);
		boolFixPosition = FALSE;
		StartTrack3 = TRUE;
		bWindowFixed = FALSE;
	    gbl_Window = this;
		this->ShowToolBox(); 
		boolStartTrackingShow = FALSE;
		StartTracking();
		return 0;  // Let the system set the focus
	}


LRESULT CToolBox::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
PAINTSTRUCT PaintStr;
	BeginPaint(&PaintStr);
 HBITMAP   hBmp = LoadBitmap(m_InstanceApp,MAKEINTRESOURCE(IDB_SCREEN));
 BITMAP     bm;


		 long mleft=-1;
		 long mtop= -1;

		 ::GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bm);


		 long w = bm.bmWidth;
		 long h = bm.bmHeight;

				  HDC hdc = ::GetWindowDC(this->m_hWnd);

   HDC hdcTemp = CreateCompatibleDC(hdc);
   SelectObject(hdcTemp, hBmp);   // Select the bitmap

  BitBlt(hdc, 0, 0, w, h, hdcTemp, 0, 0,
          SRCCOPY);


  ::ReleaseDC(this->m_hWnd,hdc);
DeleteObject(hBmp);
DeleteObject(hdcTemp);

	ShowSpire();


EndPaint(&PaintStr);
	return 0;
}

void CToolBox::ShowToolBox(void)
{


		 long mleft=-1;
		 long mtop= -1;



		 long w = 44;
		 long h = 414;





		 RECT rcScreen;
         SystemParametersInfo(SPI_GETWORKAREA, 0, &rcScreen, 0);
		 hwndToolBar = this->m_hWnd;

//		 RECT rc;
//         GetWindowRect(&rc);



	
		   mleft = GetSystemMetrics(SM_CXSCREEN) - w;
		   mtop = (GetSystemMetrics(SM_CYSCREEN) - h)/2;
	
                  SetWindowPos(HWND_NOTOPMOST, 
            mleft ,
            mtop ,
            w , h, SWP_NOZORDER) ;


SetWorkWindowRGN();




//				HWND thwnd = ::GetDlgItem(this->m_hWnd,IDC_EDIT_TEXT);
//				HDC phDc = ::GetWindowDC(thwnd);   
//				      iFontSixe = MulDiv(10, GetDeviceCaps(phDc, LOGPIXELSY), 72);
//					  ::ReleaseDC (thwnd,phDc);
//         int kj = (31 - 1.5* iFontSixe+2*GetSystemMetrics(SM_CYBORDER)) /2;
//                 ::SetWindowPos(thwnd, NULL, 
//            24 ,
//           30,
//            121 , 1.5* iFontSixe+2*GetSystemMetrics(SM_CYBORDER), SWP_NOZORDER) ;
//				 minMaxToolBar = FALSE; // max

	


}

LRESULT CToolBox::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
int xPos = GET_X_LPARAM(lParam); 
int yPos = GET_Y_LPARAM(lParam); 

          POINT pt;
   if (boolStartTrackingShow == FALSE ){
          GetCursorPos(&pt);

          RECT rect;
          GetWindowRect(&rect);
		  long W_wind = GetSystemMetrics(SM_CXSCREEN);
		  if (rect.right > W_wind ){
                 boolStartTrackingShow = TRUE;
			  StartShowTooolBox();
		  } else {
              boolStartTrackingShow = TRUE;
              StartTracking();
          }
   }
	return 0;
}

	void CToolBox::ShowUnFixWindow(void)
	{
HBITMAP   hBmp = LoadBitmap(m_InstanceApp,MAKEINTRESOURCE(IDB_SPIRE_OF));
 BITMAP     bm;


		 long mleft=-1;
		 long mtop= -1;

		 ::GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bm);


		 long w = bm.bmWidth;
		 long h = bm.bmHeight;

				  HDC hdc = ::GetWindowDC(this->m_hWnd);

   HDC hdcTemp = CreateCompatibleDC(hdc);
   SelectObject(hdcTemp, hBmp);   // Select the bitmap

  BitBlt(hdc, 24, 55, w, h, hdcTemp, 0, 0,
          SRCCOPY);


  ::ReleaseDC(this->m_hWnd,hdc);
DeleteObject(hBmp);
DeleteObject(hdcTemp);
	
};


void CToolBox::ShowFixedWindow(void)
{
HBITMAP   hBmp = LoadBitmap(m_InstanceApp,MAKEINTRESOURCE(IDB_SPIRE_ON));
 BITMAP     bm;


		 long mleft=-1;
		 long mtop= -1;

		 ::GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bm);


		 long w = bm.bmWidth;
		 long h = bm.bmHeight;

				  HDC hdc = ::GetWindowDC(this->m_hWnd);

   HDC hdcTemp = CreateCompatibleDC(hdc);
   SelectObject(hdcTemp, hBmp);   // Select the bitmap

  BitBlt(hdc, 24, 55, w, h, hdcTemp, 0, 0,
          SRCCOPY);


  ::ReleaseDC(this->m_hWnd,hdc);
DeleteObject(hBmp);
DeleteObject(hdcTemp);
}

LRESULT CToolBox::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
int xPos = GET_X_LPARAM(lParam); 
int yPos = GET_Y_LPARAM(lParam); 
if (( xPos > 24 ) && ( xPos < 43) && ( yPos > 55 ) && ( yPos < 70 )){
	if (boolFixPosition == TRUE ){
	  boolFixPosition = FALSE;
	} else {
       boolFixPosition = TRUE;
	}
  ShowSpire();
}



	return 0;
}

void CToolBox::ShowSpire(void)
{
	if (boolFixPosition == TRUE ){
				ShowFixedWindow();
						bWindowFixed = TRUE;
						KillTimer(1); 
			} else {
				ShowUnFixWindow();
						bWindowFixed = FALSE;
		                boolStartTrackingShow = FALSE;
		                StartTracking();
			}	
}




 VOID CALLBACK
   TrackMouseTimerProc(HWND hWnd,UINT uMsg,UINT idEvent,DWORD dwTime) {
	   if (idEvent == 2 ){
          RECT rect;
          GetWindowRect(hWnd,&rect);
		  long W_wind = GetSystemMetrics(SM_CXSCREEN)- 26;
		  if (rect.left < W_wind ){
			  ::SetWindowPos(hWnd,HWND_NOTOPMOST, 
            rect.left + 3 ,
            rect.top  ,
            rect.right - rect.left , rect.bottom - rect.top , SWP_NOZORDER) ;
		  } else {
             KillTimer(hWnd,idEvent); 
			 gbl_Window->SetHideRGN();
			 boolStartTrackingShow = FALSE;
		  }
	   }
      if (idEvent == 3 ){
          RECT rect;
          GetWindowRect(hWnd,&rect);
		  long W_wind = GetSystemMetrics(SM_CXSCREEN)-2;
		  if (rect.right > W_wind ){
			  int newLeft = rect.left - 3;
			  if (rect.right - 3 < GetSystemMetrics(SM_CXSCREEN)){
				  newLeft = GetSystemMetrics(SM_CXSCREEN) - (rect.right - rect.left);
            KillTimer(hWnd,idEvent); 
	 		 StartTrack3 = TRUE;
			 boolStartTrackingShow = FALSE;
			  }
			  ::SetWindowPos(hWnd,HWND_NOTOPMOST, 
            newLeft ,
            rect.top  ,
            rect.right - rect.left , rect.bottom - rect.top , SWP_NOZORDER) ;
		  } else {
             KillTimer(hWnd,idEvent); 
	 		 StartTrack3 = TRUE;
			 boolStartTrackingShow = FALSE;
		  }
	   }
  

//--------- start traching hide

	   if (idEvent == 1 ){
          RECT rect;

          POINT pt;

          GetWindowRect(hWnd,&rect);
		  rect.left -=5;
		  rect.right +=5;
          GetCursorPos(&pt);
			if (!PtInRect(&rect,pt) /*|| (WindowFromPoint(pt) != hWnd)*/) {
			  KillTimer(hWnd,idEvent); 
			  boolStartTrackingShow = FALSE;
			  gbl_Window->StartHide();
			}
		 }
   }


void CToolBox::StartHide(void)
{
	if ( bWindowFixed == FALSE) {
	  this->SetTimer(2,50,TrackMouseTimerProc);
	}
}

void CToolBox::StartTracking(void)
{
	tools_hWnd = this->m_hWnd; 
		if ( bWindowFixed == FALSE) {
	this->SetTimer(1,200,TrackMouseTimerProc);
		}
}

LRESULT CToolBox::OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default

	return 0;
}

void CToolBox::SetWorkWindowRGN(void)
{
		POINT lpptS[30];

		lpptS[0].x = 35; 
		lpptS[0].y = 0;


		lpptS[1].x = 32; 
		lpptS[1].y = 2;


		lpptS[2].x = 32; 
		lpptS[2].y = 42;


		lpptS[3].x = 29; 
		lpptS[3].y = 50;


		lpptS[4].x = 26; 
		lpptS[4].y = 59;


		lpptS[5].x = 24; 
		lpptS[5].y = 70;


		lpptS[6].x = 21; 
		lpptS[6].y = 85;



		lpptS[7].x = 21; 
		lpptS[7].y = 329;


		lpptS[8].x = 24; 
		lpptS[8].y = 345;


		lpptS[9].x = 25; 
		lpptS[9].y = 351;


		lpptS[10].x = 26; 
		lpptS[10].y = 354;


		lpptS[11].x = 27; 
		lpptS[11].y = 357;


		lpptS[12].x = 28; 
		lpptS[12].y = 361;


		lpptS[13].x = 30; 
		lpptS[13].y = 365;


		lpptS[14].x = 32; 
		lpptS[14].y = 371;


		lpptS[15].x = 32; 
		lpptS[15].y = 411;


		lpptS[16].x = 34; 
		lpptS[16].y = 413;


		lpptS[17].x = 40; 
		lpptS[17].y = 413;


		lpptS[18].x = 44; 
		lpptS[18].y = 413;

		lpptS[19].x = 44; 
		lpptS[19].y = 0;


		lpptS[20].x = 35; 
		lpptS[20].y = 0;


		lpptS[21].x = 21; 
		lpptS[21].y = 55;


		lpptS[22].x = 17; 
		lpptS[22].y = 51;


		lpptS[23].x = 17; 
		lpptS[23].y = 31;


		lpptS[24].x = 13; 
		lpptS[24].y = 30;


		lpptS[25].x = 0; 
		lpptS[25].y = 21;


		lpptS[26].x = 7; 
		lpptS[26].y = 0;


		lpptS[27].x = 8; 
		lpptS[27].y = 0;



		lpptS[28].x = 8; 
		lpptS[28].y = 0;



		lpptS[29].x = 8; 
		lpptS[29].y = 0;




			 HRGN rgn4 = CreatePolygonRgn(lpptS,20, ALTERNATE);

/* HRGN rgnE = CreateRoundRectRgn(
  1,      // x-coordinate of upper-left corner
  14,       // y-coordinate of upper-left corner
  36,     // x-coordinate of lower-right corner
  53,    // y-coordinate of lower-right corner
  39,  // height of ellipse
  39  // width of ellipse
);
*/
  
//CombineRgn(rgnE,rgn4,rgnE,RGN_OR);

		    int tmp = SetWindowRgn(rgn4, TRUE);

}

void CToolBox::SetHideRGN(void)
{
//	RECT rect;
//	::GetWindowRect(this->m_hWnd ,&rect);
//	HRGN rgnE = CreateRectRgn(rect.left,rect.top,rect.right,rect.bottom);    
//			    int tmp = SetWindowRgn(rgnE, FALSE);
}

void CToolBox::StartShowTooolBox(void)
{
	RECT rect;
          GetWindowRect(&rect);

		this->SetTimer(3,50,TrackMouseTimerProc);

}

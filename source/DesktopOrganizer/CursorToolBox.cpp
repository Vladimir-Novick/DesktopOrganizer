// CursorToolBox.cpp : Implementation of CCursorToolBox

#include "stdafx.h"
#include "CursorToolBox.h"
#include ".\cursortoolbox.h"
#include "MDragTarget.h"


// CCursorToolBox

static 	long ToolBocCursorbmWidth;
static 	long ToolBocCursorbmHeight;

VOID CALLBACK
   TrackMouseTimerCursorProc(HWND hWnd,UINT uMsg,UINT idEvent,DWORD dwTime) {
 
   }



	LRESULT CCursorToolBox::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<CCursorToolBox>::OnInitDialog(uMsg, wParam, lParam, bHandled);
		MakeScreenSize();

		RECT RectCursor;

          ClipCursor(&RectCursor);


		  POINT	m_CursorPoint;
		  GetCursorPos(&m_CursorPoint);

          RECT rect;
		  ::GetWindowRect(this->m_hWnd,&rect);
		  long W_wind = GetSystemMetrics(SM_CXSCREEN)- 26;
		  ::SetWindowPos(this->m_hWnd,HWND_NOTOPMOST, 
            m_CursorPoint.x + 15  ,
            m_CursorPoint.y + 15  ,
            rect.right - rect.left , rect.bottom - rect.top , SWP_NOZORDER) ;
		   ShowWindow(SW_HIDE);


		                pIDropTarget=new CDropTarget(this);
			  if (NULL!=pIDropTarget) 
			  {    
				  OleInitialize(NULL);
				  pIDropTarget->AddRef();
			    CoLockObjectExternal(pIDropTarget,
				  TRUE, FALSE);
			    RegisterDragDrop(m_hWnd, pIDropTarget);
                CoDisconnectObject(pIDropTarget,0);
			  }

		return 1;  // Let the system set the focus
	}



LRESULT CCursorToolBox::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
PAINTSTRUCT PaintStr;
	BeginPaint(&PaintStr);
 HBITMAP   hBmp = LoadBitmap(m_InstanceApp,MAKEINTRESOURCE(IDB_CURSORTOOLBARSCREEN));
 BITMAP     bm;


		 long mleft=-1;
		 long mtop= -1;

		 ::GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bm);


		 long w = bm.bmWidth;
		 long h = bm.bmHeight;

		 ToolBocCursorbmWidth  = bm.bmWidth;
	     ToolBocCursorbmHeight = bm.bmHeight;

				  HDC hdc = ::GetWindowDC(this->m_hWnd);

   HDC hdcTemp = CreateCompatibleDC(hdc);
   SelectObject(hdcTemp, hBmp);   // Select the bitmap

  BitBlt(hdc, 0, 0, w, h, hdcTemp, 0, 0,
          SRCCOPY);


  ::ReleaseDC(this->m_hWnd,hdc);
DeleteObject(hBmp);
DeleteObject(hdcTemp);





	return 0;
}

void CCursorToolBox::MakeScreenSize(void)
{
	POINT lpptS[30];

		lpptS[0].x = 0; 
		lpptS[0].y = 8;


		lpptS[1].x = 21; 
		lpptS[1].y = 0;


		lpptS[2].x = 24; 
		lpptS[2].y = 7;


		lpptS[3].x = 30; 
		lpptS[3].y = 0;


		lpptS[4].x = 33; 
		lpptS[4].y = 0;


		lpptS[5].x = 33; 
		lpptS[5].y = 2;


		lpptS[6].x = 26; 
		lpptS[6].y = 10;



		lpptS[7].x = 33; 
		lpptS[7].y = 25;


		lpptS[8].x = 33; 
		lpptS[8].y = 27;


		lpptS[9].x = 14; 
		lpptS[9].y = 34;


		lpptS[10].x = 10; 
		lpptS[10].y = 34;


		lpptS[11].x = 0; 
		lpptS[11].y = 10;


		lpptS[12].x = 0; 
		lpptS[12].y = 8;


		lpptS[13].x = 0; 
		lpptS[13].y = 2;


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




			 HRGN rgn4 = CreatePolygonRgn(lpptS,12, ALTERNATE);

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

LRESULT CCursorToolBox::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	RevokeDragDrop(this->m_hWnd); 

	return 0;
}

LRESULT CCursorToolBox::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	      RECT RectCursor;

		  if (ClipCursor(&RectCursor)!= 0){
			  RectCursor.left = 0;
			  RectCursor.right = 15;
			  RectCursor.top = 0;
			  RectCursor.bottom = 15;
		  }


		  POINT	m_CursorPoint;
		  GetCursorPos(&m_CursorPoint);

          RECT rect;
		  ::GetWindowRect(this->m_hWnd,&rect);
		  ::SetWindowPos(this->m_hWnd,HWND_NOTOPMOST, 
            m_CursorPoint.x - ToolBocCursorbmWidth   ,
            m_CursorPoint.y + RectCursor.bottom - RectCursor.top   ,
            rect.right - rect.left , rect.bottom - rect.top , SWP_NOZORDER) ;

	return 0;
}

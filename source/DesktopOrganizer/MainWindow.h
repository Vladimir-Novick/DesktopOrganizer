// MainWindow.h : Declaration of the CMainWindow

#pragma once

#include "resource.h"       // main symbols
#include <atlhost.h>
#include "ToolBox.h"


// CMainWindow


struct ButtonTypeStruct { 
	int left;
	int leftTop;
	int right;
	int Buttom;
	int IDC_image_on;
	int IDC_image_of;
};

class CMainWindow : 
	public CAxDialogImpl<CMainWindow>
{
public:
	CToolBox *m_TollBox;
    HWND menu_hwnd;
	HWND hwndToolBar;
	CMainWindow();


	~CMainWindow()
	{
	}

	enum { IDD = IDD_MAINWINDOW };

BEGIN_MSG_MAP(CMainWindow)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	menu_hwnd = hWnd;
	MESSAGE_HANDLER(UM_DISPRESICONNOTIFY, OnTryMessage)
	MESSAGE_HANDLER(WM_DRAWITEM,ShowMenuItem_WM_DRAWITEM)
	MESSAGE_HANDLER(WM_MEASUREITEM,ShowMenuItem_WM_MEASUREITEM)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
	MESSAGE_HANDLER(WM_COPYDATA, OnCopyData)
	CHAIN_MSG_MAP(CAxDialogImpl<CMainWindow>)

END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		ShowWindow(SW_HIDE);
		
		 //
		 // Close Button
		 //
		 MyButton[1].left = 150;
		 MyButton[1].leftTop = 15;
		 MyButton[1].right = 160;
		 MyButton[1].Buttom = 25;
	     MyButton[1].IDC_image_on = 0;
	     MyButton[1].IDC_image_of = 0;

		 // Minimaize Button
		 //
		 MyButton[2].left = 129;
		 MyButton[2].leftTop = 13;
		 MyButton[2].right = 144;
		 MyButton[2].Buttom = 26;
	     MyButton[2].IDC_image_on = 0;
	     MyButton[2].IDC_image_of = 0;

				        hImageList = NULL;

		return 0;
	}


	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTryMessage (UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	void CreateMenuListRBotton(HMENU hmenuMain,HMENU hmenuContext);
	void DrawTransparentBitmap(HDC hdc, HBITMAP hBitmap, long xStart,
                           long yStart, COLORREF cTransparentColor);
    LRESULT ShowMenuItem_WM_MEASUREITEM(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT ShowMenuItem_WM_DRAWITEM(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    HFONT GetAFontA(int fnFont);
	ButtonTypeStruct MyButton[15];
	BOOL dwn;
		HIMAGELIST hImageList;
LRESULT OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
BOOL AppTrackStart(void);
void AppTrackExit(void);
LRESULT OnCopyData(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};



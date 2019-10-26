// DesktopOrganizer.cpp : Implementation of WinMain

#include "stdafx.h"
#include "resource.h"
#include "MainWindow.h"



// The module attribute causes WinMain to be automatically implemented for you
//[ module(EXE, uuid = "{07B4F6AF-5CE2-4072-A7AB-9BF34827F276}", 
//		 name = "DesktopOrganizer", 
//		 helpstring = "DesktopOrganizer 1.0 Type Library",
//		 resource_name = "IDR_DesktopOrganizer") ]
//
CMainWindow m_MainWindow;
HINSTANCE m_InstanceApp;

class CDesktopOrganizerModule : public CAtlExeModuleT< CDesktopOrganizerModule >
{
public:
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DesktopOrganizer, "{07B4F6AF-5CE2-4072-A7AB-9BF34827F276}")
	
	HRESULT PreMessageLoop(int nShowCmd)
	{
		HRESULT hr = CAtlExeModuleT<CDesktopOrganizerModule>::PreMessageLoop(nShowCmd);
		if (FAILED(hr))
			return hr;

HWND m_Wnd =  FindWindow(
  NULL,  // class name
  "Desktop Organaizer"  // window name
);




   if (m_Wnd != NULL ) {

      MessageBox(NULL, __TEXT("Desktop Organaizer is already running"), 
         _TEXT("Desktop Organaizer"), MB_ICONINFORMATION | MB_OK);

   } else {


 




	   m_MainWindow.DoModal(::GetDesktopWindow());
   }

  
		
		return S_FALSE;
	}
};

CDesktopOrganizerModule _AtlModule;

/////////////////////////////////////////////////////////////////////////////
//
extern "C" int WINAPI _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	m_InstanceApp = hInstance;
	_AtlModule.WinMain(nShowCmd);
}

		 

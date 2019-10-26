

//*****************************************************************************/

#include <windows.h>
#include "trackdll.h"
#include <stdio.h>


#define DBG

#define MK_CONTROL = 0x8


 SPYMSGDATA gsmd;
// COPYDATASTRUCT gcds = { 0, sizeof(SPYMSGDATA), &gsmd };
HANDLE	hInstance;				// Global instance handle for	DLL
#pragma data_seg("SHARDATA")
     static volatile BOOL flsendK = FALSE;
     static volatile BOOL flsendKBOOK = FALSE;
     static HHOOK hhkMouseMessage = NULL;
	 static HHOOK ghwndSpyHookKeyBoard = NULL;
     static HHOOK hhkGetMessage = NULL;
     static HHOOK hhkCallWndProc = NULL;
     static HWND ghwndSpyHook = NULL;   // the handle back to the spy executable
     MSG pmsg;
     static volatile BOOL flControl = FALSE;
     static volatile BOOL flControlBOOK = FALSE;
	 
#pragma data_seg()

//#ifdef DBG
//VOID DbgPrintf(LPTSTR fmt, ...);
//#endif



/*****************************************************************************\
* DllMain (hModule,cbHeap,lpchCmdLine)
*
* Called when the libary is loaded
*
* Arguments:
*    PVOID hModule - Module handle for the libary.
*    ULONG ulReason - DLL purpose
*    PCONTEXT pctx - not used
*
* Returns:
*    TRUE - Everything is ok
*    FALSE- Error.
\*****************************************************************************/

BOOL
APIENTRY DllMain(
    PVOID hModule,
    ULONG ulReason,
    PCONTEXT pctx
    )
{
    //
    // This function is called for every instance of the DLL. We must find
    // and store the handle to the spy window every time an instance of the
    // DLL is instantiated.
    //
    if ( ulReason == DLL_PROCESS_ATTACH ) {
    hInstance = hModule;
					flsendK = FALSE;
		 		   flControl = FALSE;
					flsendKBOOK = FALSE;
		 		   flControlBOOK = FALSE;
    }

    return TRUE;
}




/*****************************************************************************\
* SetSpyHook
*
* Sets the windows hooks used to trap the messages.  After this
* is called with a TRUE for fSet, the messages will start flowing
* through the hook DLL.
*
* Arguments:
*   BOOL fSet - TRUE to hook, FALSE to unhook.
*
* Returns:
*   TRUE if successful.
*
\*****************************************************************************/

BOOL
SetSpyHook(
    BOOL fSet
    )
{
  

    if (fSet)
    {

      if (!hhkGetMessage)
        {
            if (!(hhkGetMessage = SetWindowsHookEx(WH_GETMESSAGE,
                (HOOKPROC)SpyGetMsgProc, hInstance, 0)))
            {
                return FALSE;
            }
        }

        if (!hhkCallWndProc)
        {
            if (!(hhkCallWndProc = SetWindowsHookEx(WH_CALLWNDPROC,
                SpyCallWndProc, hInstance, 0)))
            {
                UnhookWindowsHookEx(hhkGetMessage);
                return FALSE;
            }
        }


        if (!hhkMouseMessage)
        {
            if (!(hhkMouseMessage = SetWindowsHookEx(WH_MOUSE,
                MouseFunc, hInstance, 0)))
            {
                UnhookWindowsHookEx(hhkGetMessage);
                UnhookWindowsHookEx(hhkCallWndProc);
 
                return FALSE;
            }
        }


        if (!ghwndSpyHookKeyBoard)
        {
            if (!(ghwndSpyHookKeyBoard = SetWindowsHookEx( WH_KEYBOARD,
                KeyboardFunc, hInstance, 0)))
            {
                UnhookWindowsHookEx(hhkGetMessage);
                UnhookWindowsHookEx(hhkCallWndProc);
                UnhookWindowsHookEx(hhkMouseMessage);
 
                return FALSE;
            }
        }


		

    }
    else
    {
        if (hhkGetMessage)
        {
            UnhookWindowsHookEx(hhkGetMessage);
            hhkGetMessage = NULL;
        }

        if (hhkCallWndProc)
        {
            UnhookWindowsHookEx(hhkCallWndProc);
            hhkCallWndProc = NULL;
        }


        if (hhkMouseMessage)
        {
            UnhookWindowsHookEx(hhkMouseMessage);
            hhkMouseMessage = NULL;
        }


        if (ghwndSpyHookKeyBoard)
        {
            UnhookWindowsHookEx(ghwndSpyHookKeyBoard);
            ghwndSpyHookKeyBoard = NULL;
        }



		

    }

    return TRUE;
}



LRESULT CALLBACK	InstallHook(HWND target_hWnd ){
	ghwndSpyHook = target_hWnd;
          return SetSpyHook(TRUE);
}
LRESULT CALLBACK	UninstallHook(){
	          return SetSpyHook(FALSE);
}


/*****************************************************************************\
* HookProc( hWnd, uiMessage, wParam, lParam )
*
* The hook proc for the windows hook being spied on
*
* Arguments:
*    HWND hWnd - window handle for the parent window
*    UINT uiMessage - message number
*    WPARAM wParam - message-dependent
*    LPARAM lParam - message-dependent
*
* Returns:
*    0 if processed, nonzero if ignored
\*****************************************************************************/

BOOL WINAPI
HookProc(
    HWND hwnd,
    UINT uiMessage,
    WPARAM wParam,
    LPARAM lParam
    )
{

  
    if (ghwndSpyHook != NULL && hwnd != ghwndSpyHook)
    {

        //
        // Send the message on asynchronously for Spy to deal with if
        // it is the appropriate hwndSpyingOn window to spy on.
        //

            gsmd.wParam = wParam;
            gsmd.lParam = lParam;

            gcds.dwData = uiMessage;
            SendMessage(ghwndSpyHook, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)&gcds);


    }

    return FALSE;
}







// KeyboardFunc    17 - ctrl ; 18- alt
// lparam < 0 
//
// Filter function for the WH_KEYBOARD
//
//--------------------------------------------------------------------------

LRESULT CALLBACK KeyboardFunc (int nCode, WPARAM wParam, LPARAM lParam )
{
    static int index = 1;
   if ( nCode >= 0 ) {


                     gsmd.wParam = wParam;
                     gsmd.lParam = lParam;

                     gcds.dwData = wParam;
                     SendMessage(ghwndSpyHook, WM_COPYDATA, wParam, (LPARAM)&gcds);


}
  //	return 1L;
   //
   // We looked at the message ... sort of processed it but since we are
   // looking we will pass all messages on to CallNextHookEx.
   //
   return( CallNextHookEx(ghwndSpyHookKeyBoard, nCode, wParam,lParam));

}




//--------------------------------------------------------------------------
// MouseFunc
//
// Filter function for the WH_MOUSE
//
//--------------------------------------------------------------------------
LRESULT CALLBACK MouseFunc (int nCode, WPARAM wParam, LPARAM lParam )
{

   LPMOUSEHOOKSTRUCT MouseHookParam;

	static int index = 1;


   if ( nCode >= 0 ) {

//      if ( nCode == HC_NOREMOVE )
//		 strcpy(szType, "NOT Removed from Queue");
//      else
//		 strcpy(szType, "REMOVED from Queue
// ");

	  MouseHookParam = (MOUSEHOOKSTRUCT *) lParam;

	  if (MouseHookParam->hwnd != ghwndSpyHook ){


            gsmd.wParam = wParam;
            gsmd.lParam = lParam;
            gcds.dwData = wParam;
            SendMessage(ghwndSpyHook, WM_COPYDATA, (WPARAM)MouseHookParam->hwnd,  (LPARAM)&gcds);

	  }


   }



   //
   // We looked at the message ... sort of processed it but since we are
   // looking we will pass all messages on to CallNextHookEx.
   //
   return( CallNextHookEx(hhkMouseMessage, nCode, wParam, lParam));
}


/*****************************************************************************\
* SpyGetMsgProc
*
* The Get Message hook function.
*
\*****************************************************************************/

LRESULT CALLBACK
SpyGetMsgProc(
    INT hc,
    WPARAM wParam,
    LPARAM lParam
    )
{
    PMSG pmsg;

    pmsg = (PMSG)lParam;

    if (hc >= 0 && pmsg && pmsg->hwnd)
    {
         HookProc(pmsg->hwnd, pmsg->message, pmsg->wParam, pmsg->lParam);

         return CallNextHookEx(hhkGetMessage, hc, wParam, lParam);

    }

    //
    // Note that CallNextHookEx ignores the first parameter (hhook) so
    // it is acceptable (barely) to pass in a NULL.
    //
    return CallNextHookEx(NULL, hc, wParam, lParam);
}



/*****************************************************************************\
* SpyCallWndProc
*
* The Call Window Proc (Send Message) hook function.
*
\*****************************************************************************/

LRESULT CALLBACK
SpyCallWndProc(
    INT hc,
    WPARAM wParam,
    LPARAM lParam
    )
{
    PCWPSTRUCT pcwps;

    pcwps = (PCWPSTRUCT)lParam;

    if (hc >= 0 && pcwps && pcwps->hwnd)
    {
              HookProc(pcwps->hwnd, pcwps->message, pcwps->wParam, pcwps->lParam);
		       return CallNextHookEx(hhkCallWndProc, hc, wParam, lParam);
    }

    //
    // Note that CallNextHookEx ignores the first parameter (hhook) so
    // it is acceptable (barely) to pass in a NULL.
    //
    return CallNextHookEx(NULL, hc, wParam, lParam);
}



#ifdef DBG
/****************************************************************************
* DBGprintf
*
* This debugging function prints out a string to the debug output.
* An optional set of substitutional parameters can be specified,
* and the final output will be the processed result of these combined
* with the format string, just like printf.  A newline is always
* output after every call to this function.
*
* Arguments:
*   LPTSTR fmt - Format string (printf style).
*   ...        - Variable number of arguments.
* Returns:
*    VOID
\****************************************************************************/

VOID DbgPrintf(
    LPTSTR fmt,
    ...
    )
{
    va_list marker;
    TCHAR szBuf[256];

    va_start(marker, fmt);
    wvsprintf(szBuf, fmt, marker);
    va_end(marker);

    OutputDebugString(szBuf);
    OutputDebugString(TEXT("\r\n"));
}
#endif



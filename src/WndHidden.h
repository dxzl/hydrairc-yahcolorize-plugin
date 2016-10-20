// CoLoRiZe Plugin by Scott Swift, 2015
// I am the author and put this under GPL v3 - S.S.
// www.YahCoLoRiZe.com
//
// WinHidden.cpp : Creates a hidden window that processes WM_COPYDATA

#pragma once


//************************************************************************************
// Getting timer messages into an ATL object isn't all that evident. If you create
// a full control and it remains invisible in runtime, it has no windows handle, so you
// can't use the classical ::SetTimer(hWnd,...) to get the timer messages.
//
// The trick is to create another ATL window object that does absolutely nothing
// but create a window and get these messages. Then have the timer message handler
// call whatever method you want in your "real" ATL object.
//
// The extra ATL window class looks like the following. I place it in the same file
// as the main ATL class, but before it, adding a forward class declaration for the
// main class, of course. In the constructor it takes a pointer to the ATL object that
// needs timer ticks.

class CPlugin;

// ATL_NO_VTABLE is a Microsoft-specific optimization that tells the compiler:
// "this class is never used by itself, but only as a base class for other classes,
// so don't create extra code to manage a table of function-pointers."
// (so we don't want that here...!)
//public ATL_NO_VTABLE CWindowImpl<CHydraColorizeMsgPump, CWindow, CNullTraits>

class CHydraColorizeMsgPump : public CWindowImpl<CHydraColorizeMsgPump, CWindow, CNullTraits>
{
  private: // default

    CPlugin* m_pPlugin;
    char* m_data; // pointer to allocated memory for our received string
    UINT m_TimerID;
    int m_channelID, m_serverID, m_clientID;

    void StopTimer(void);
    void MemDelete(void);

    BEGIN_MSG_MAP(CHydraColorizeMsgPump)
      MESSAGE_HANDLER(WM_COPYDATA, OnCopyData)
      MESSAGE_HANDLER(WM_TIMER, OnTimer)
    END_MSG_MAP()

    HWND GetColorizeHandle(void);
    char* GetChanOrQueryName(int ID);

  public:

    DECLARE_WND_CLASS(_T("CHydraColorizeMsgPump")) 

    // TODO: Organize vars into properties!
    LRESULT OnTimer(UINT, WPARAM, LPARAM, BOOL&);
    LRESULT OnCopyData(UINT, WPARAM, LPARAM, BOOL&);
    void OnFinalMessage(HWND); // override

    HWND m_hWndColorize; // YahCoLoRiZe's handle
    UINT m_ColorizeNet;

    // Constructor
    CHydraColorizeMsgPump(CPlugin* pPlugin);

    BOOL SendColorize(int ServerID, int ChanQueryID, int CommandID, int IntData, char* pData);
};

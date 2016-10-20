// CoLoRiZe Plugin by Scott Swift, 2015
// I am the author and put this under GPL v3 - S.S.
// www.YahCoLoRiZe.com
//
// WinHidden.cpp : Creates a hidden window that processes WM_COPYDATA

#include "stdafx.h"
#include <HydraIRCCommon.h>
#include <API_Basic.h>
//#include <API_GUI.h>
//#include <API_CommandProcessor.h>

extern PluginBasicTable __BASICTABLENAME;

//Put what we are using into our scope
using std::string;

//***************************************************************************

CHydraColorizeMsgPump::CHydraColorizeMsgPump(CPlugin* pPlugin) : m_pPlugin(pPlugin)
{
  m_data = NULL;
  m_TimerID = 0;

  m_channelID = -1;
  m_serverID = -1;

  // Register Class Name
  //CString wnd_class_name = ::AfxRegisterWndClass(NULL);
  //BOOL created = CWnd::CreateEx(0,wnd_class_name, 
  //  "CHydraColorizeMsgPump",0 ,0 ,0 ,0 ,0 ,HWND_MESSAGE,0);

  // Register Windows Message
  m_ColorizeNet = RegisterWindowMessage(WMS_COLORIZENET);
  m_hWndColorize = GetColorizeHandle();
};

LRESULT CHydraColorizeMsgPump::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  // Don't re-trigger! (we really need a one-shot Timer :)
  StopTimer();

  // Post to Main Hydra and clear data-object
  if (m_data)
  {
    // copy the pointer
    char* pData = m_data;

    int len = strlen(pData);

    char s[7];

    // so much for "string", C# .NET and UTF-8 or Unicode... back to a little
    // old-school ASCII shoveling :)

    if (len > 5) // /msg x
    {
      strncpy(s, pData, 5);
      m_pPlugin->strlower(s);

      if (!strncmp(s, "/msg ", 5))
      {
        // Diagnostic
        // sys_Printf(BIC_INFO, "%i", m_serverID);

        pData += 5; // point past "/msg "

        // point to first non-space (beginning of channel name)
        while(*pData == ' ') pData++;

        char* pText = strstr(pData, " "); // point to space after channel (null if not found)

        if (pText) // Must always have a channel-name and a space!
        {
          *pText++ = '\0'; // Split into channel and text, point to text

          if (m_serverID < 0 && m_channelID < 0 && *pData != '\0')
            // Shame you can't set server -1 or 0 and it would find the 1st channel
            // on any server matching pData :)
            HydraIRC_CMD_PrivMsg(1, pData, pText); // send by channel name ONLY
          else if (m_serverID > 0)
          {
            if (m_channelID == 0)
              HydraIRC_ServerPrintf(m_serverID, BIC_INFO, pText); // print to server window
            else if (m_channelID > 0)
              HydraIRC_CMD_PrivMsg(m_channelID, NULL, pText); // send by channel ID
            else if (*pData != '\0')
              // Colorize sent -1, use the channel-name it passed in command-string...
              HydraIRC_CMD_PrivMsg(m_serverID, pData, pText); // send by channel name
            else
              HydraIRC_ServerPrintf(m_serverID, BIC_INFO, pText); // print to server window
          }
          else
            sys_Printf(BIC_INFO, pText); // output window
        }
      }
      // Look for /echo command (there is no channel for a YahCoLoRiZe /echo)
      else if (len > 6) 
      {
        strncpy(s, pData, 6);
        m_pPlugin->strlower(s);

        if (!strncmp(s, "/echo ", 6))
        {
          char* pText = pData+6;

          if (m_serverID < 0)
            HydraIRC_ServerPrintf(1, BIC_INFO, pText); // print to server window
          else if (m_serverID > 0)
          {
            if (m_channelID <= 0)
              HydraIRC_ServerPrintf(m_serverID, BIC_INFO, pText); // print to server window
            else
              HydraIRC_CMD_PrivMsg(m_channelID, NULL, pText); // send by channel ID
          }
          else
            sys_Printf(BIC_INFO, pText); // output window
        }
      }
    }

    //// BIC = Buffer Item Contents
    //#define BIC_UNKNOWN     0
    //
    //// IRC Server/Channel Output Types
    //#define BIC_NOTICE      1
    //#define BIC_TOPIC       2
    //#define BIC_JOIN        3
    //#define BIC_KICK        4
    //#define BIC_MODE        5
    //#define BIC_NICK        6
    //#define BIC_PART        7
    //#define BIC_ACTION      8
    //#define BIC_QUIT        9
    //#define BIC_CONNECTING  10
    //#define BIC_CHANNELTEXT 11
    //#define BIC_QUERYTEXT   12
    //#define BIC_SERVERMSG   13
    //#define BIC_CTCP        14
    //#define BIC_OWNTEXT     15
    //#define BIC_WALLOPS     16
    //
    //// HydraIRC output types
    //#define BIC_CONSOLEFIRST 100
    //
    //#define BIC_FUNCTION     (BIC_CONSOLEFIRST + 0)
    //#define BIC_SOCKET       (BIC_CONSOLEFIRST + 1)
    //#define BIC_WARNING      (BIC_CONSOLEFIRST + 2)
    //#define BIC_ERROR        (BIC_CONSOLEFIRST + 3)
    //#define BIC_GUI          (BIC_CONSOLEFIRST + 4)
    //#define BIC_INFO         (BIC_CONSOLEFIRST + 5)
    //#define BIC_XML          (BIC_CONSOLEFIRST + 6)
    //#define BIC_CONSOLELAST  (BIC_CONSOLEFIRST + 7)
    //*********************************************************************************
    // THIS WORKS in color in the default server window with no connection...
    // server ID is 1... need to strip "/msg #room "
    // (does not send over the network!)
    //if (m_serverID >= 0)
    //  HydraIRC_ServerPrintf(1, BIC_INFO, m_data);
    //else
    //  HydraIRC_ServerPrintf(1, BIC_INFO, m_data);

    // THIS WORKS in color in the #Scott channel...
    // ID is 2... need to strip "/msg #room "
    // (does not send over the network!)
    //HydraIRC_ChannelPrintf(2, BIC_INFO, m_data);

    // THIS WORKS! (this prints to our window and across the network!)
    // Send it to #Scott over the network!
    // Pass: (Window ID, "To", "String")
    // Pass: (Channel ID, NULL, "String")
    //HydraIRC_CMD_PrivMsg(2, NULL, m_data);

    // This WORKS - sends over the server to #Scott but we DO NOT see it
    // locally!!!!!
    //HydraIRC_SendRaw(2, "PRIVMSG #Scott :hi this is a test!\r\n");

    // THIS WORKS - prints an "action" like /me goes out to dinner! (in a special color)
    //HydraIRC_CMD_Action(2, m_data);

    // This prints non-color to the Output window at the bottom...
    //sys_Printf(BIC_INFO, m_data);
    //*********************************************************************************

    MemDelete();

    // Important to clear these and allow YahCoLoRiZe to send
    // the channel name it you set in the Play->Options menu!
    m_serverID = -1;
    m_channelID = -1;
  }

  return 0;
}

LRESULT CHydraColorizeMsgPump::OnCopyData(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  m_hWndColorize = (HWND)wParam; // sender's handle is in wParam

  COPYDATASTRUCT * pCds = reinterpret_cast<COPYDATASTRUCT *>(lParam);

  UINT customWinMsg = (UINT)pCds->dwData; // Message WMS_COLORIZENET

  if ((int)pCds->cbData != sizeof(COLORIZENET_STRUCT)) // Wrong structure-size...
  {
    bHandled = FALSE;
    return 0;
  }
  
  COLORIZENET_STRUCT * pCns = reinterpret_cast<COLORIZENET_STRUCT *>(pCds->lpData);

  if (customWinMsg != m_ColorizeNet) // Wrong custom-message...
  {
    bHandled = FALSE;
    return 0;
  }
  
  int size = pCns->lenData; // The length of the string in characters
  int command = pCns->commandID;

  m_channelID = pCns->channelID;
  m_serverID = pCns->serverID;
  m_clientID = pCns->clientID;

  if (!size || size > CNS_DATALEN-1 || 
          command != REMOTE_COMMAND_TEXT || m_clientID != HYDRA_CLIENT_ID)
  {
    // Wrong string size or wrong remote command or this is the wrong client...
    bHandled = FALSE;
    return 0;
  }

  if (m_data)
  {
    // Previous message has not been processed yet... reset everything
    StopTimer();
    MemDelete();
  }

  m_data = new char[size+1];
  strncpy(m_data, (char *)pCns->data, size); // copy string
  m_data[size] = '\0'; // add a null terminator!

  if (!m_data)
  {
    bHandled = FALSE;
    return 0;
  }

  // Set timer to handle the bulk of processing so we release the WM_COPYDATA memory!
  m_TimerID = SetTimer(IDT_TIMER1, 5, NULL); // Set Timer1 for 5ms

  bHandled = TRUE;
  return 0;
}

void CHydraColorizeMsgPump::OnFinalMessage(HWND hWnd)
// Override... Called when our hidden messaging window gets WM_NCDESTROY
// NOTE: When the plugin unloads we SendMessage WM_CLOSE to WndHidden which
// initiates a WM_NCDESTROY!
{
  // Clean up...
  StopTimer();
  MemDelete();
  //sys_Printf(BIC_INFO, "Deleting WndHidden!");
  delete this; // Goodbye!
}

void CHydraColorizeMsgPump::StopTimer(void)
{
  if (m_TimerID)
  {
    KillTimer(m_TimerID);
    m_TimerID = 0;
  }
}

void CHydraColorizeMsgPump::MemDelete(void)
{
  if (m_data)
  {
    delete [] m_data;
    m_data = NULL;
  }
}

  // ChannelID < 0 if it's a Query but no good as an ID
  // ChannelID == 0 if no channels
  // ChannelID == 2 for 1st channel, 3 for second... (internal window IDs?)
  // ServerID is 1 on startup and one all the time if 1 server connected.
  //
  // Modified main code-file ChildFrm.cpp to send a m_QueryID as a negative integer!
  // We won't have any Name if we type into the status window!
  //  if (!(CI.Mask & HIRC_CI_NAME)) // Got channel name ?
  //    return FALSE;
  // Print name!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //if (ChannelID > 0)
  //  sys_Printf(BIC_INFO, CI.Name);


BOOL CHydraColorizeMsgPump::SendColorize(int ServerID, int ChanQueryID, int CommandID, int IntData,
                                                                      char* pData)
{
  m_hWndColorize = GetColorizeHandle();

  if (!m_hWndColorize) return FALSE;

  COPYDATASTRUCT* cds = new COPYDATASTRUCT();
  COLORIZENET_STRUCT* cns = new COLORIZENET_STRUCT();

  if (pData)
  {
    // Buffer size needed to hold Command
    cns->lenData = strnlen(pData, CNS_DATALEN-1);

    if (cns->lenData)
    {
      if (cns->lenData > CNS_DATALEN-1)
        cns->lenData = CNS_DATALEN-1;

      strncpy((char *)cns->data, pData, cns->lenData); // copy string, pad with 0's
    }

    cns->data[cns->lenData] = '\0'; // add a null terminator!
  }

  char* pChanNick = GetChanOrQueryName(ChanQueryID);
  
  if (pChanNick)
  {
    cns->lenChanNick = strlen(pChanNick);

    if (cns->lenChanNick)
    {
      if (cns->lenChanNick > CNS_CHANNICKLEN-1)
        cns->lenChanNick = CNS_CHANNICKLEN-1;

      strncpy((char *)cns->chanNick, pChanNick, cns->lenChanNick); // copy string, pad with 0's
    }

    cns->chanNick[cns->lenChanNick] = '\0'; // add a null terminator!
    //sys_Printf(BIC_INFO, "CHANNEL ID:::::::::: %i, %s", ChanQueryID, cns->chanNick);
  }

  // REMOTE_COMMAND_START   0
  // REMOTE_COMMAND_STOP    1
  // REMOTE_COMMAND_PAUSE   2
  // REMOTE_COMMAND_RESUME  3
  // REMOTE_COMMAND_CHANNEL 4
  // REMOTE_COMMAND_TIME    5
  // REMOTE_COMMAND_ID      6
  // REMOTE_COMMAND_FILE    7
  // REMOTE_COMMAND_TEXT    8 // only command we receive

  cns->commandID = (__int32)CommandID; // Command ID (-1 if not set)
  cns->commandData = (__int32)IntData; // Integer data for command (-1 if not set)
  cns->serverID = (__int32)ServerID; // Server ID (-1 if not set)
  cns->channelID = (__int32)ChanQueryID; // Channel ID (-1 if not set)
  cns->clientID = (__int32)HYDRA_CLIENT_ID;

  cds->dwData = m_ColorizeNet; // Unique windows message we registered
  cds->cbData = sizeof(COLORIZENET_STRUCT); // size of data
  cds->lpData = cns; // pointer to data

  DWORD result;
  ::SendMessageTimeout(m_hWndColorize, WM_COPYDATA, reinterpret_cast<WPARAM>(this->m_hWnd),
    reinterpret_cast<LPARAM>(cds), SMTO_BLOCK | SMTO_ABORTIFHUNG, 5000, &result);

  delete cns;
  delete cds;

  return TRUE;
}

HWND CHydraColorizeMsgPump::GetColorizeHandle(void)
{
  // Find the YahCoLoRiZe main window (if app is running)...
  HWND hWnd = ::FindWindow(YAHCOLORIZE_CLASSNAME, YAHCOLORIZE_WINDOWNAME);
  if (hWnd == NULL) // Try the class-name alone...
      hWnd = ::FindWindow(YAHCOLORIZE_CLASSNAME, NULL);
  if (hWnd == NULL) // Try the window-name alone...
      hWnd = ::FindWindow(NULL, YAHCOLORIZE_WINDOWNAME);
  return hWnd;
}

char* CHydraColorizeMsgPump::GetChanOrQueryName(int ID)
// Return pointer to allocated memory with name of channel or nickname of query
// Returns NULL if no memory is created.
//
// On entry we expect ID to be < 0 if it's an m_QueryID and > 0 if it's a channel ID
// *** Dont forget to "delete [] p" the memory!!!!!!!
{
  if (ID == 0) return NULL;

  int len = 0;
  char* p = NULL;

  if (ID < 0)
  {
    HIRC_QueryInfo_t QI;
    HydraIRC_GetQueryInfo(-ID,&QI);
    if (!(QI.Mask & HIRC_QI_OTHERNICK)) return NULL;
    if ( (len = strlen(QI.OtherNick)) == 0) return NULL;
    p = new char[len+1];
    strncpy(p,QI.OtherNick,len);
  }
  else
  {
    HIRC_ChannelInfo_t CI;
    HydraIRC_GetChannelInfo(ID,&CI);
    if (!(CI.Mask & HIRC_CI_NAME)) return NULL;
    if ( (len = strlen(CI.Name)) == 0) return NULL;
    p = new char[len+1];
    strncpy(p,CI.Name,len);
  }

  p[len] = '\0';

  return p;
}
//***************************************************************************

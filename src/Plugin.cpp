/*

  NOTE: The main Readme.md for the original HydraIRC repository states:
  
  "The license for the source is now GPL v3 - many of the source code files still
  refer to the old license, these references are outdated as of 1st October 2014
  and will be replaced with the GPL header."

  I wrote this plugin for a heavily modified version of HydraIRC for Visual Studio
  2015
  
  CoLoRiZe Plugin by Dxzl, 2015
  www.YahCoLoRiZe.com

  FYI: Here is the original header for the plugin template-file:
  
  HydraIRC
  Copyright (C) 2002-2006 Dominic Clifton aka Hydra

  HydraIRC limited-use source license

  1) You can:
  1.1) Use the source to create improvements and bug-fixes to send to the
       author to be incorporated in the main program.
  1.2) Use it for review/educational purposes.

  2) You can NOT:
  2.1) Use the source to create derivative works. (That is, you can't release
       your own version of HydraIRC with your changes in it)
  2.2) Compile your own version and sell it.
  2.3) Distribute unmodified, modified source or compiled versions of HydraIRC
       without first obtaining permission from the author. (I want one place
       for people to come to get HydraIRC from)
  2.4) Use any of the code or other part of HydraIRC in anything other than 
       HydraIRC.
       
  3) All code submitted to the project:
  3.1) Must not be covered by any license that conflicts with this license 
       (e.g. GPL code)
  3.2) Will become the property of the author.

*/

// Plugin.cpp : An example HydraIRC plugin
//

#include "stdafx.h"
#include <HydraIRCCommon.h> // Has HIRC_ChannelInfo_s and HIRC_ServerInfo_s
#include <API_Basic.h>
//#include <API_GUI.h>
#include <API_CommandProcessor.h>

#include "Plugin.h"

//Put what we are using into our scope
using std::string;

//CAppModule _Module;

// If we didn't create this, __BASICTABLENAME would be #defined in API_Basic.h as
// g_PluginBasicTable.
PluginBasicTable __BASICTABLENAME;

CPlugin Plugin;

// return TRUE if command has been processed to half further processing
//BOOL Plugin_ProcessCommand(int ServerID, int ChanQueryID, char **Command, char **Args)
//{
//  BOOL Processed = FALSE;
//
//  return Processed;
//}

// return TRUE if command has been processed to half further processing
//BOOL Plugin_ProcessChannelCommand(int ServerID, int ChannelID, char **Command, char **Args)
//{
//  BOOL Processed = FALSE;
//
//  return Processed;
//}

// return TRUE if command has been processed to half further processing
//BOOL Plugin_ProcessQueryCommand(int ServerID, int QueryID, char **Command, char **Args)
//{
//  BOOL Processed = FALSE;
//
//  return Processed;
//}

// return TRUE if command has been processed to half further processing
BOOL Plugin_ProcessInput(int ServerID, int ChanQueryID, char **Command)
  // Modified main code-file ChildFrm.cpp to send a m_QueryID as a negative
  // integer in the ChannelID var! (Search for "S.S.")
  //
  // return TRUE if command has been processed...
{  
  char *pCopy = strdup(*Command); // we need to work on a copy of the command

  if (!pCopy) return FALSE; // can't continue without our copy..

  BOOL Processed = FALSE;

  char* pCmd = strtok(pCopy," ");

  if (pCmd && *pCmd == '/')
  {
    char* pArg = strtok(NULL," ");

    Plugin.strlower(++pCmd); // skip over the '/' character and convert to lowercase

    // Handle the command "/cx blah blah" that sends text to YahCoLoRiZe.exe which
    // then processes it and sends it back via WM_COPYDATA to our message-handler
    // in WndHidden.cpp where we post it to the original channel it was targeting.
    if (pArg)
    {
      if (!strcmp(pCmd,"cx"))
      {
        Processed = TRUE;
        Plugin.pWndHidden->SendColorize(ServerID, ChanQueryID, REMOTE_COMMAND_TEXT, -1,
                                                                    *Command+(pArg-pCmd+1));
      }
      else if (!strcmp(pCmd,"cplay"))
      {
        Processed = TRUE;
        Plugin.strlower(pArg);

        if (!strcmp(pArg,"start"))
          Plugin.pWndHidden->SendColorize(ServerID, ChanQueryID, REMOTE_COMMAND_START, -1, NULL);
        else if (!strcmp(pArg,"stop"))
          Plugin.pWndHidden->SendColorize(ServerID, ChanQueryID, REMOTE_COMMAND_STOP, -1, NULL);
        else if (!strcmp(pArg,"pause"))
          Plugin.pWndHidden->SendColorize(ServerID, ChanQueryID, REMOTE_COMMAND_PAUSE, -1, NULL);
        else if (!strcmp(pArg,"resume"))
          Plugin.pWndHidden->SendColorize(ServerID, ChanQueryID, REMOTE_COMMAND_RESUME, -1, NULL);
        else // File-path
          Plugin.pWndHidden->SendColorize(ServerID, ChanQueryID, REMOTE_COMMAND_FILE, -1,
                                                                     *Command+(pArg-pCmd+1));
      }
      else if (!strcmp(pCmd,"cchan"))
      {
        Processed = TRUE;
        Plugin.pWndHidden->SendColorize(ServerID, ChanQueryID, REMOTE_COMMAND_CHANNEL, -1, pArg);
      }
      else if (!strcmp(pCmd,"ctime"))
      {
        Processed = TRUE;
        Plugin.pWndHidden->SendColorize(ServerID, ChanQueryID, REMOTE_COMMAND_TIME, -1, pArg);
      }
    }
    else if (!strcmp(pCmd,"chelp"))
    {
      Processed = TRUE;
      HydraIRC_ServerPrintf(ServerID, BIC_INFO,"*************************************************");
      HydraIRC_ServerPrintf(ServerID, BIC_INFO,"* CoLoRiZe Plugin Help... www.yahcolorize.com");
      HydraIRC_ServerPrintf(ServerID, BIC_INFO,"*************************************************");
      HydraIRC_ServerPrintf(ServerID, BIC_INFO,"* First, run YahCoLoRiZe and in the");
      HydraIRC_ServerPrintf(ServerID, BIC_INFO,"* \"Client\" menu choose \"Hydra\".");
      HydraIRC_ServerPrintf(ServerID, BIC_INFO,"* Next, in Hydra type: /cx TEST123.");
      HydraIRC_ServerPrintf(ServerID, BIC_INFO,"*");
      HydraIRC_ServerPrintf(ServerID, BIC_INFO,"* Examples:");
      HydraIRC_ServerPrintf(ServerID, BIC_INFO,"* /cx 0 My Text (random text-effect)");
      HydraIRC_ServerPrintf(ServerID, BIC_INFO,"* /cx 12 My Text (uses text-effect 12)");
      HydraIRC_ServerPrintf(ServerID, BIC_INFO,"* /cchan #MyRoom (channel YahCoLoRiZe sends to)");
      HydraIRC_ServerPrintf(ServerID, BIC_INFO,"* /ctime 3000 (sets time-per-line to 3 seconds)");
      HydraIRC_ServerPrintf(ServerID, BIC_INFO,"* /cplay C:\\CHAT\\MyFile.txt (plays file)");
      HydraIRC_ServerPrintf(ServerID, BIC_INFO,"* /cplay start|stop|pause|resume");
      HydraIRC_ServerPrintf(ServerID, BIC_INFO,"*");
      HydraIRC_ServerPrintf(ServerID, BIC_INFO,"* *NOTE: Use the \"Courier New\" font for");
      HydraIRC_ServerPrintf(ServerID, BIC_INFO,"* even text-borders!");
      HydraIRC_ServerPrintf(ServerID, BIC_INFO,"*************************************************");
    }
  }

  free(pCopy);

  return Processed;
}

/*
 *	Actual Plugin -> HydraIRC functions start here
 *
 */

PLUGIN_API HydraIRCPlugin *HydraIRCPlugin_Init(void)
{
  return &Plugin;
}

/*
 *	Here's the plugin interface.
 */

CPlugin::CPlugin( void )
{ 
  m_Name = "CoLoRiZe"; // only use alpha-numeric, -'s and _'s and NO SPACES
  m_Author = "Dxzl";
  m_Info = VERSION; // __DATE__

  // Init class members
  pWndHidden = NULL;
  m_hWndHidden = 0;

	return; 
}

// GetAPI is a function in HydraIRCPlugin.lib which both the main program and us link with.
// It is only called by us, not by main Hydra. It is used to REPLACE main-program
// handlers with our own (an "override" of sorts).

// First HydraIRCPlugin_Init is called which returns our pointer, a HydraIRCPlugin type.
// Then the main program calls this. AddAPI is a function in HydraIRCPlugin.lib, which
// we link with.
BOOL CPlugin::RequestAPIs( void )
{
  if (AddAPI(PLUGIN_API_CMDPROC,"Type /chelp for command-list!", sizeof(PluginCmdProcTable)) &&
      AddAPI(PLUGIN_API_BASIC,NULL,sizeof(PluginBasicTable),PLUGIN_REQUIRES,&__BASICTABLENAME))
    return TRUE;
  else
    return FALSE;
}

// AddAPI is a function in HydraIRCPlugin.lib which both the main program and us link with.
// It is called by both us and main Hydra. It is used below to REPLACE main-program
// handlers with our own (an "override" of sorts).
// 
// Note that we don't impliment "case PLUGIN_API_BASIC:" - that's done by
// PluginManager.cpp when it calls its own GetAPI(). It sets all the available functions
// in the table we provided, __BASICTABLENAME. We could ovveride certain functions
// below if we needed to.
BOOL CPlugin::GetAPI(APIDescriptor_t *pAPID)
{
  switch (pAPID->m_API)
  {
    case PLUGIN_API_CMDPROC:
      PluginCmdProcTable* pTable = static_cast<PluginCmdProcTable*>(pAPID->m_pTable);

      // fill in the table with only the functions we implement.
      pTable->m_pfnProcessInput = Plugin_ProcessInput;
      //pTable->m_pfnProcessQueryInput =;
      //pTable->m_pfnProcessCommand =;
      //pTable->m_pfnProcessChannelCommand =;
      //pTable->m_pfnProcessServerCommand =;
      //pTable->m_pfnProcessDCCInput =;

      return TRUE;
  }
  return FALSE;
}

void CPlugin::OnActivate( void )
{
#ifdef DEBUG
  OutputDebugString("CPlugin::OnActivate() called\n");
#endif

  // http://www.codeproject.com/Articles/7969/How-to-make-a-Message-Only-Window

  if ((pWndHidden = new CHydraColorizeMsgPump(this)) != NULL)
    m_hWndHidden = pWndHidden->Create(NULL);

  if (!(pWndHidden && m_hWndHidden))
  {
    sys_Printf(BIC_INFO,"Error loading the CoLoRiZe.dll plugin\n");
    return;
  }

  pWndHidden->SetWindowText("HydraMsgPump");

  if (!pWndHidden->SendColorize(-1, -1, REMOTE_COMMAND_ID, -1, VERSION))
    sys_Printf(BIC_INFO,"Start YahCoLoRiZe then type /cx <yourtext>\n");

  sys_Printf(BIC_INFO,"%s Plugin by %s Activated\n", m_Name, m_Author);
}

void CPlugin::OnDeactivate( void )
{
#ifdef DEBUG
  OutputDebugString("CPlugin::OnDeactivate() called\n");
#endif

  if (m_hWndHidden)
  {
    SendMessage(m_hWndHidden, WM_CLOSE, 0, 0);
    m_hWndHidden = 0;
  }
}

BOOL CPlugin::OnRequestUnload( void )
{
#ifdef DEBUG
  return TRUE;
  // example of how you can ask the user if your plugin is busy doing stuff
  //return (IDYES == MessageBox(NULL,"Allow Plugin.DLL Unload?", NULL, MB_YESNO | MB_ICONQUESTION));
#else
  return TRUE;
#endif
}
/**************************************************************************************\
                                YahCoLoRiZe functions
\**************************************************************************************/

void CPlugin::strlower(char* p)
{ 
  while(*p != '\0')
  {
    *p = tolower(*p);
    p++;
  }
}

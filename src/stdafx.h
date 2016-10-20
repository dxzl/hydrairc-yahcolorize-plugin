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

  CoLoRiZe Plugin by Scott Swift, 2013
  www.YahCoLoRiZe.com

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

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WINVER		0x0500
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0300

// S.S. Add this in atldef.h to keep newer atl from calling LCMapStringEx() (which is only on Vista and up...)
#define _ATL_XP_TARGETING

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <atlbase.h>

// TODO: reference additional headers your program requires here
// ATL/GUI includes start here

//#include <atlapp.h>
//#include <atlframe.h>
//#include <atlctrls.h>
//#include <atlctrlx.h>
//#include <atldlgs.h>
//#include <atlctrlw.h>
//#include <atlsplit.h>
//#include <atlmisc.h>
//#include <afxwin.h> // for AfxRegisterWndClass

#include <atlcom.h>
//#include <commctrl.h> // for HIMAGELIST
#include <atlwin.h>
#include <atlapp.h>
#include <atluser.h>
#include <atlctrls.h>
#include <CList.h>
#include <string>
#include <HydraIRCPlugin.h>
#include "Plugin.h"
#include "WndHidden.h"

//extern CAppModule _Module;

// enable shorthand use of functions
#define USE_BASICTABLE_DEFINE

// We send our handle and this version-string when HydraIRC loads us...
#define VERSION "1.5" // Compatible with our format for other client-plugins

// After receiving WM_COPYDATA from YahCoLoRiZe, we allocate memory
// and put the received string there, set a timer and return promptly
// from the call.  The timer triggers a seperate event that sends the info
// to Hydra-Main and frees the memory.
#define IDT_TIMER1 1

#define CHAN_BUFSIZE 64

#define HYDRA_CLIENT_ID 4 // permanent client ID assigned by YahCoLoRiZe to HydraIRC

#define YAHCOLORIZE_CLASSNAME "TDTSColor"
#define YAHCOLORIZE_WINDOWNAME "YahCoLoRiZe"

// Max size (in bytes) of message to send to/from YahCoLoRiZe
#define CNS_DATALEN 2048
// Max size (in bytes) of channel or nickname
#define CNS_CHANNICKLEN 512

// Registered Windows Messages
#define WMS_COLORIZENET "WM_ColorizeNet"

// Remote commands to YahCoLoRiZe
#define REMOTE_COMMAND_START   0
#define REMOTE_COMMAND_STOP    1
#define REMOTE_COMMAND_PAUSE   2
#define REMOTE_COMMAND_RESUME  3
#define REMOTE_COMMAND_CHANNEL 4
#define REMOTE_COMMAND_TIME    5
#define REMOTE_COMMAND_ID      6
#define REMOTE_COMMAND_FILE    7
#define REMOTE_COMMAND_TEXT    8 // we both send and receive this on /cx handshake

// Struct to xfer text between clients and YahCoLoRiZe
struct COLORIZENET_STRUCT
{
  __int64 lspare;
  __int32 clientID;
  __int32 commandID;
  __int32 commandData;
  __int32 serverID;
  __int32 channelID;
  __int32 lenChanNick; // length of channel or nickname string in characters, without the null
  __int32 lenData; // length of data string in characters, without the null
  BYTE chanNick[CNS_CHANNICKLEN];
  BYTE data[CNS_DATALEN];

  // Auto-Initialize
  COLORIZENET_STRUCT(void)
  {
    lspare = 0;
    clientID = -1;
    commandID = -1;
    commandData = -1;
    serverID = -1;
    channelID = -1;
    lenChanNick = 0;
    lenData = 0;
    chanNick[0] = 0;
    data[0] = 0;
  }
};

// Alt Message Map ID
#define THID_COPYDATA 1

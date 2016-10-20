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

#pragma once

class CHydraColorizeMsgPump;

class CPlugin : public HydraIRCPlugin
{
private:

  HWND CPlugin::GetColorizeHandle(void);

public:

  CHydraColorizeMsgPump* pWndHidden;
  HWND m_hWndHidden;

  CPlugin(void);
  ~CPlugin(void) { };

  BOOL RequestAPIs( void );
  void OnActivate( void );
  void OnDeactivate( void );
  BOOL OnRequestUnload( void );

  BOOL GetAPI(APIDescriptor_t *pAPID);

  void strlower(char* p);
};

PLUGIN_API HydraIRCPlugin *Init(void);
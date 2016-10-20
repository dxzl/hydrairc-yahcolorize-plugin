Put this directory in:

\My Documents\Visual Studio 2015\Projects\HydraIRC\HydraIRC\Plugins

You will need to point the library and include paths to wherever ATLMFC is
in your system. For me, I copied the files from the Windows Driver SDK - ATL
(Active Template Library) "may" not come with Visual Studio 2015 Community!

Building HydraIRC was a genuine PAIN IN THE #$#%$# and is beyond the scope of what I
can get into here. I knew nothing about ATL and walked straight into a mess! But
write me at e-mail below and I'll be happy to help.

Many thanks to Dominic (aka Hydra) for all of his ground-breaking hard work on the
original project. The entire project needs to be UTF-8 Unicode enabled - my own
project, YahCoLoRiZe is now fully Unicode and works with LeafChat (a Java IRC client),
IceChat (a C# client) and of course, mIRC.

dxzl - Important note: For each plugin you need to add PLUGIN_EXPORTS
to the Preprocessor definitions in Properties->C/C++. Don't add it to
HydraIRCPlugin or the main HydraIRC project. PLUGIN_EXPORTS defines our plugin
as PROVIDING exports when we link with HydraIRCPlugin.lib. The the main program
also links with HydraIRCPlugin.lib but as RECEIVING imports do don't define
it for the main project...

The stuff below is all I had to go on...

Dxzl: dxzl@live.com
      www.yahcolorize.com

========================================================================
    DYNAMIC LINK LIBRARY : Plugin Project Overview
========================================================================

AppWizard has created this Plugin DLL for you.  
This file contains a summary of what you will find in each of the files that
make up your Plugin application.


Plugin.vcproj
    This is the main project file for VC++ projects generated using an Application Wizard. 
    It contains information about the version of Visual C++ that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

Plugin.cpp
    This is the main DLL source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named Plugin.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////

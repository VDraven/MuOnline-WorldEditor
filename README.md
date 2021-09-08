# MuOnline-WorldEditor  
  
This is an in-game WorldEditor for Mu Online Season 16e1 (Main version : 1.19.46)  
(No support for other versions)  
  
If you want to ask or look for help, please don't open issues here. Because it will not be processed.  
Please post it on ragezone or my original thread instead.  
https://forum.ragezone.com/f197/release-source-mu-online-world-1199965/  
  
### This project has been completed (SEP 2021).  
(The external data will NOT be implemented further, due to incompatibility)  
  
https://youtu.be/629J5BmGAXQ  
[![WorldEditor](https://img.youtube.com/vi/629J5BmGAXQ/hqdefault.jpg)](https://youtu.be/629J5BmGAXQ)  
  
[Tutorial] Loading World
https://www.youtube.com/watch?v=uM_FCUafc5E  
[![Tut1](https://img.youtube.com/vi/uM_FCUafc5E/hqdefault.jpg)](https://youtu.be/uM_FCUafc5E)  


Main 1.19.46 unpacked by me can be found here:  
https://github.com/VD1988/MuOnline_S16_Lgd/tree/main/Main  
  
It's been testing with server files released by MuLegend.  
For server installation helps, please visit:  
https://forum.ragezone.com/f197/release-legend-mu-client-amp-1195391/  
  
Beside the server, you also need to add codes to load the WorldEditor dynamic library into client's Main.dll source (also released by MuLegend).  
Copy these lines to Main.cpp :  
```
	HINSTANCE hInstance = LoadLibraryA("Plugins\\WorldEditor.dll");
	if (!hInstance) 
		MessageBoxA(NULL, "Could not load the WorldEditor.dll", "ERROR", MB_OK);
```
Or hooking directly to main.exe is also OK.  
Then remember to copy-paste "Plugins" folder to the client location.  
```
//  MuOnline-Client
//  |- main.exe
//  |- Main.dll
//  |--Plugins
//    |-WorldEditor.dll
//    |-Config.ini
//    ...
//
```

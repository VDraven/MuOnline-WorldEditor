## MuOnline-WorldEditor  
This is an in-game WorldEditor for Mu Online Season 16e1 (Main version : 1.19.46)  
It's been testing with server files released by MuLegend.  
For server installation helps, please visit "https://forum.ragezone.com/f197/release-legend-mu-client-amp-1195391/".  
  
Beside the server, you also need to add codes to load the WorldEditor dynamic library into client's Main.dll source (also released by MuLegend).
Copy these lines to Main.cpp :  
```
	HINSTANCE hInstance = LoadLibraryA("Plugins\\WorldEditor.dll");
	if (!hInstance) 
		MessageBoxA(NULL, "Could not load the WorldEditor.dll", "ERROR", MB_OK);
```
Or hooking directly from main.exe is also OK.
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

# This project is not completed.

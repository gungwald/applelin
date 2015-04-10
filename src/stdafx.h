//#define WIN32_LEAN_AND_MEAN

// Mouse Wheel is not supported on Win95.
// If we didn't care about supporting Win95 (compile/run-time errors)
// we would just define the minmimum windows version to support.
// #define _WIN32_WINDOWS  0x0401
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#endif

//#include <crtdbg.h>
//#include <dsound.h>
//#include <dshow.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <tchar.h>
#include <time.h>

/* Define an internal macro that identifies UNIX or UNIX-like systems. */
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
#define UNIX_STYLE_OS
#endif

#ifdef UNIX_STYLE_OS
/* Added to fix bugs 1,2 & 3 from SourceForge. I experienced the bug on
   Debian 7.6 on x86_64 but not on Debian 7.6 for PowerPC. I'm not sure
   why there is a difference. They should have the same compiler and
   headers. - Bill Chatfield 2015-04-09 */
#include <unistd.h>     /* usleep, read, write, close */
#include "wincompat.h"
#else
#include <windows.h>
#endif

//#include <winuser.h> // WM_MOUSEWHEEL
//#include <commctrl.h>
//#include <ddraw.h> - no need
//#include <htmlhelp.h> - no need

#include <SDL.h>
#include <SDL_endian.h>

/* This automatically figures out what the host's byte order is and
   converts the Apple II byte order, which is little endian, to
   the host's byte order, whatever it may be. So this does the right
   thing for both PowerPC (big endian) and x86 (little endian) hosts.
   This should be used like the TCP/IP network function ntohs. */
#define APPLE2_2BYTE_ORDERING_TO_HOST(x) SDL_SwapLE16(x)
#define APPLE2_4BYTE_ORDERING_TO_HOST(x) SDL_SwapLE32(x)


#include "Common.h"
#include "Structs.h"

#include "AppleWin.h"
#include "AY8910.h"

#include "CPU.h"

#include "Debug.h" //- may be soon...


#include "Disk.h"
#include "DiskChoose.h"
#include "DiskImage.h"

#include "Frame.h"
#include "Harddisk.h"
#include "Joystick.h"
#include "Keyboard.h"
#include "Log.h"
#include "Memory.h"
#include "Mockingboard.h"
#include "ParallelPrinter.h"
//#include "PropertySheetPage.h"
#include "Registry.h"

#include "Riff.h"
#include "SaveState.h"
#include "SerialComms.h"
#include "SoundCore.h"  //- will do later
#include "Speaker.h"
//#include "Tfe/Tfe.h"

#include "stretch.h"

#include "Timer.h"

#include "Video.h"

//#include "Util_MemoryTextFile.h"


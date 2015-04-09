This is a continuation of LinApple 2.0 which I have named AppleLin
to differentiate it.

Goals
* Apply existing patches
* Fix bugs
* Make it work on Linux PowerPC
* Figure out how to merge with AppleWin so that the Linux version can benefit from work being done there


What follows is the original README from LinApple 2.0:
----------------------------------------------------------------------
*** LINAPPLE 2.0***

[INTRO]
What is linapple?
It is an emulator of Apple2 (Apple][, Apple 2, Apple 2e etc.) computer series for Linux or other OSes with SDL support.

(SDL means Simple DirectMedia Layer written by Sam Lantinga and others).

linapple works out of the box, just make it once and run whenever you wish.

Why not AppleLin? Yes, at first I think to call it such, but later linapple seemed better to me. And one point!

This is my work for making it possible to have comprehensive emulator of Apple][ computer series in Linux.

The original source is from AppleWin (http://applewin.berlios.de) by Tom Charlesworth and others.

I just adapted it for: video+audio for SDL, other functions for POSIX compliant OS, which Linux is in particularly.

Later some sort of Windows support had been added, so you may try to compile it under Windows.



[INSTALL]

I will show brief instructions for compiling and installing linapple here.

Please, see INSTALL file for comprehensive installing tips.

After SDL1.2, zlib, libcurl and libzip are installed in your system, you can build sources.

Unpack downloaded linapple archive and go to src directory:

$ cd src

Then run:

$ make

or

$ sudo make install

If there is no errors, run:

$ linapple

or without being installed:

$ cd ..
$ ./linapple

If all files are on their places, you should see an X-Window with splash screen. 

Press F2 (or F3 or Alt+F3) before to choose some disk image in drive 1), and go to work.

It works as well as AppleWin, although it needs testing and supporting.
Where to install? Anywhere you feel fit,  just let linapple be with some files it needs for comprehenceive working.

These files are:

splash.bmp 			- splash screen
charset40.bmp		- charset for Apple][ (Apple 2e, etc.) text modes.
font.bmp			- font for Help screen and Disk Select screens.
icon.bmp			- nice icon, logo of Apple][ computer.
linapple.conf		- configuration file.
Master.dsk			- disk image with Applesoft(tm) DOS 3.3 inside. See Apple license (on apple.com) for details.


[USING LINAPPLE]

At first start you should see some nice splashscreen. Press F2 to start emualtion. Master.dsk should load, and you will see an DOS3.3(tm) prompt. By pressing F3 you may select an image file name (usually having extensions .dsk, .do, .po) into Slot 6 Drive 1. After selecting it, you either may reboot anew (by pressing F2 key) and try to boot from the inserted disk, or type any DOS3.3(tm) commands to view disk contents and to run any program located in it. Common DOS commands are `CATALOG' (like `ls' in Linux), `BRUN FILENAME' - running any binary (B) program, or `RUN FILENAME' - running any Basic program (A or I). Usually, most game images have alternative way to run programs in them. You may find a lot of disk images on largest and oldest ftp for Apple][ stuff ftp.apple.asimov.net.

From version 2 of linapple you are able to download such images directly from FTP! Use Alt+F3 or Alt+F4 for charging drive 1 and drive 2 for slot 6 (if you know what I am about :) ) - they are just common disk sources of Apple 2 computer for reading/writing its files, such as system, games, utilities and so on.

To quit emualtor peacefully, press F10. F6 toggles fullscreen mode, Scroll Lock toggles fullspeed mode.

Other possible key commands of linapple are the following:

     F1 - Show help screen
     F2 - Cold reset
     Shift+F2 - Reload conf file and restart
     F3, F4 - Choose an image file name for floppy disk in Slot 6 drive 1 or 2 respectively
     Shift+F3, Shift+F4 - The same thing for Apple hard disks (in Slot 7)
	Alt+F3,Alt+F4 - same as F3,F4 using FTP (see linapple.conf about configuring FTP accounts)
	Alt+Shift+F3, Alt+Shift+F4 - same as Shift+F3, Shift+F4 but using FTP account (see above)

     F5 - Swap drives for Slot 6
     F6 - Toggle fullscreen mode
     F7 - Reserved for Debugger!
     F8 - Save current screen as a .bmp file
     Shift+F8 - Save settings changable in runtime in conf file
     
     F9 - Cycle through various video modes
     F10 - Quit emulator

     F11 - Save current state to file, Alt+F11 - quick save
     F12 - Reload it from file, Alt+F12 - quick load

     Ctrl+0..9	- fast load state snapshot with corresponding number, saved previously by
     Ctrl+Shift+0..9 - fast save snapshot to current snapshot directory with corresponding number 0..9

     Ctrl+F12 - Hot reset

      Pause - Pause emulator
      Scroll Lock - Toggle full speed
    Num pad keys:
     Grey + - Speed up emulator
     Grey - - Speed it down
     Grey * - Normal speed

Other powerful way to control behaviour of linapple is to change different settings in linapple.conf file, which should be located in the same directory where linapple is. Please, see linapple.conf contents to know what each setting means.

Note one point, too. When mouse works as a joystick, or Mouse Interface is on, when you first left-click on the window of emulator, mouse cursor is grabbed for using inside it. To release cursor, you should click left-mouse button again with SHIFT or CTRL key pressed. In fullscreen mode mouse cursor is always grabbed, to release it you may want to switch to windowed mode by pressing F6 key.

In version 1.1 appeared VideoBenchmark: start linapple with -b command-line key to see its results, like this:

$ ./linapple -b		- run VideoBenchmark and exit emulator. Note: if fullscreen mode is on in linapple.conf, then
			  VideoBenchmark will be runned in fullscreen mode.



In version 2 besides FTP support there is also support for changable Window sizes (see at the end of linapple.conf file for details).
Just be aware when you are not running emulator on its native mode (560x384) you can suffer slowdown if you are using old box.



[SUPPORT]
Wanted:

-- we need to get rid of some SOUND glitches. HELP!

--DEBUGGER
		I just owe at 9300 lines of code of AppleWin debugger. Michael Pohoreski, you are he-man, I tell you.
		Besides Debugger.cpp alone, there are some helper files such as Debugger_Assembler.cpp,
		Debugger_Console.cpp, Debugger_Display.cpp and so on. Who is able to port it to linapple?


--PHASOR
		Who knows how does that great thing work? Please, help adopting it to lianpple.
   		All sources for	sound support are in Mockingboard.cpp and SoundCore.cpp.
		SDL Audio used. Speakers are working out in Speaker.cpp. Who may think, heh?
		From version 1.1 Mockingboard support are done.



More testing and debugging needed, of course. I would be grateful for any comment or suggestion regarding linapple, and using it on OSes other than Linux.


[CONTACTS]
I am Krez beom beotiger (Kbb), you may contact me at your will by my email beotiger@gmail.com or beotiger@mail.ru

Web-site: http://linapple.sourceforge.net


Thank you very much.
Long Live Linux!
And Apple][!

--
24 March 2012 AD
beotiger@mail.ru


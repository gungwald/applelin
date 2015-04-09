/*
////////////////////////////////////////////////////////////////////////////
////////////  Choose disk image for given slot number? ////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//// Adapted for linapple - apple][ emulator for Linux by beom beotiger Nov 2007   /////
///////////////////////////////////////////////////////////////////////////////////////
// Original source from one of Brain Games (http://www.braingames.getput.com)
//	game Super Transball 2.(http://www.braingames.getput.com/stransball2/default.asp)
//
// Brain Games crew creates brilliant retro-remakes! Please visit their site to find out more.
//
*/
#ifdef _WIN32
#include "windows.h"
#else
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "ctype.h"
#endif

#include "stdafx.h"

#include "list.h"

#include "DiskChoose.h"

//#include "ctype.h"

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "SDL/SDL.h"
//#include "SDL_mixer.h"
//#include "SDL_image.h"

/*


#include "auxiliar.h"

#include "tiles.h"
#include "maps.h"
#include "transball.h"

#include "encoder.h"
*/



// how many file names we are able to see at once!
#define FILES_IN_SCREEN		21

// delay after key pressed (in milliseconds??)
#define KEY_DELAY		25

/////////////////////////////////////////////////////////////////////
/* FONT prev decls */
//bool fonts_initialization(void);
//void font_print(int x,int y,char *text,SDL_Surface *surface);
//void font_print_centered(int x,int y, char *text, SDL_Surface *surface);

/////////////////////////////////////////////////////////////////////
/* AUX funcs prev decls */
//void rectangle(SDL_Surface *surface, int x, int y, int w, int h, Uint32 pixel);
//void surface_fader(SDL_Surface *surface,float r_factor,float g_factor,float b_factor,float a_factor,SDL_Rect *r);


////////////////////////////////////////////////////////////////////////////////////////
#ifndef _WIN32
int getstat(char *catalog, char *fname, int * size)
{
	// gets file status and returns: 0 - special or error, 1 - file is a directory, 2 - file is a normal file
	// In: catalog - working directory, fname - file name
	struct stat info;
	char tempname[MAX_PATH];

	snprintf(tempname, MAX_PATH, "%s/%s", catalog, fname);	// get full path for the file
	if(stat(tempname, &info) == -1) return 0;
	if(S_ISDIR(info.st_mode)) return 1;	// seems to be directory
	if(S_ISREG(info.st_mode)) {
		if(size != NULL) *size = (int)(info.st_size / 1024);	// get file size in Kbytes?!
		return 2;	// regular file
	}

	return 0;
}
#endif
////////////////////////////////////////////////////////////////////////////////////////
bool ChooseAnImage(int sx,int sy, char *incoming_dir, int slot, char **filename, bool *isdir, int *index_file)
{
/*	Parameters:
 sx, sy - window size,
 incoming_dir - in what dir find files,
 slot - in what slot should an image go (common: #6 for 5.25' 140Kb floppy disks, and #7 for hard-disks).
 	slot #5 - for 800Kb floppy disks, but we do not use them in Apple][?
	(They are as a rule with .2mg extension)
 index_file	- from which file we should start cursor (should be static and 0 when changing dir)

 Out:	filename	- chosen file name (or dir name)
	isdir		- if chosen name is a directory
*/
	/* Surface: */
	SDL_Surface *my_screen;	// for background

	if(font_sfc == NULL)
		if(!fonts_initialization()) return false;	//if we don't have a fonts, we just can do none

	List<char> files;		// our files
	List<char> sizes;		// and their sizes (or 'dir' for directories)

	int act_file;		// current file
	int first_file;		// from which we output files
	printf("Disckchoose! We are here: %s\n",incoming_dir);
//	files.Delete();
//	sizes.Delete();
#ifndef _WIN32
/* POSIX specific routines of reading directory structure */
		DIR *dp;
		struct dirent *ep;

		dp = opendir (incoming_dir);	// open and read incoming directory
		char *tmp;

		int i,j, B, N;	// for cycles, beginning and end of list

// build prev dir
	if(strcmp(incoming_dir, "/")) {
		tmp = new char[3];
		strcpy(tmp, "..");
		files.Add(tmp);
		tmp = new char[5];
		strcpy(tmp, "<UP>");
		sizes.Add(tmp);	// add sign of directory
		B = 1;
	}
	else	B = 0;	// for sorting dirs
		if (dp != NULL)
		{
			while (ep = readdir (dp)) // first looking for directories
			{
				int what = getstat(incoming_dir, ep->d_name, NULL);
				if (strlen(ep->d_name) > 0 && /*strcmp(ep->d_name,".")*/// omit "." (cur dir)
					 ep->d_name[0] != '.'/*strcmp(ep->d_name,"..")*/ && what == 1) // is directory!
				{
					tmp = new char[strlen(ep->d_name)+1];	// add entity to list
					strcpy(tmp, ep->d_name);
					files.Add(tmp);
					tmp = new char[6];
					strcpy(tmp, "<DIR>");
					sizes.Add(tmp);	// add sign of directory
				} /* if */

			}
		}
#else
/* Windows specific functions of reading directory structure */
		/* Find subdirs: */ 
	if(strcmp(incoming_dir, "/")) {
	// we are not in upper direcory
		tmp = new char[3];
		strcpy(tmp, "..");
		files.Add(tmp);
		tmp = new char[5];
		strcpy(tmp, "<UP>");
		sizes.Add(tmp);	// add sign of directory
		B = 1;
	}
	else	B = 0;	// for sorting dirs
	

		WIN32_FIND_DATA finfo;
		HANDLE h;

		
		
		h=FindFirstFile(incoming_dir,&finfo);

		if (h!=INVALID_HANDLE_VALUE) {
			char *tmp;
			if(finfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY != 0) {
			// add this entry if it is directory
				tmp=new char[strlen(finfo.cFileName)+1];
				strcpy(tmp,finfo.cFileName);
				files.Add(tmp);
				tmp = new char[6];
				strcpy(tmp, "<DIR>");
				sizes.Add(tmp);	// add sign of directory
			}
			while(FindNextFile(h,&finfo)==TRUE) {
				if(finfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY != 0) {
				// add this entry if it is directory
					tmp=new char[strlen(finfo.cFileName)+1];
					strcpy(tmp,finfo.cFileName);
					files.Add(tmp);
					tmp = new char[6];
					strcpy(tmp, "<DIR>");
					sizes.Add(tmp);	// add sign of directory
				}
			} /* while */ 
		} /* if */ 

#endif
// sort directories. Please, don't laugh at my bubble sorting - it the simplest thing I've ever seen --bb
			if(files.Length() > 2)
			{
				N = files.Length() - 1;
//				B = 1;`- defined above
				for(i = N; i > B; i--)
					for(j = B; j < i; j++)
						if(strcasecmp(files[j], files[j + 1]) > 0)
						{
							files.Swap(j,j + 1);
							sizes.Swap(j,j + 1);
						}

			}
			B = files.Length();	// start for files
#ifndef _WIN32
/* POSIX specific routines of reading directory structure */

			(void) rewinddir (dp);	// to the start
				// now get all regular files
			while (ep = readdir (dp))
			{
				int fsize;

				if (strlen(ep->d_name) > 4 && ep->d_name[0] != '.'
					&& (getstat(incoming_dir, ep->d_name, &fsize) == 2)) // is normal file!
				{
					tmp = new char[strlen(ep->d_name)+1];	// add this entity to list
					strcpy(tmp, ep->d_name);
					files.Add(tmp);
					tmp = new char[10];	// 1400000KB
					snprintf(tmp, 9, "%dKB", fsize);
					sizes.Add(tmp);	// add this size to list
				} /* if */

			}
			(void) closedir (dp);
#else
/* Windows specific functions of reading directory structure */
		/* Find files: */ 

		h=FindFirstFile(incoming_dir,&finfo);

		if (h!=INVALID_HANDLE_VALUE) {
//			char *tmp; - must be defined in previous section, when searching subdirs
			if(finfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY == 0) {
			// add this entry if it is NOT directory!
				tmp=new char[strlen(finfo.cFileName)+1];
				strcpy(tmp,finfo.cFileName);
				files.Add(tmp);
				tmp = new char[10];	// 1400000KB
				snprintf(tmp, 9, "%dKB", 
					((finfo.nFileSizeHigh * (MAXDWORD+1)) + finfo.nFileSizeLow));
				sizes.Add(tmp);	// add this size to list
			}
			while(FindNextFile(h,&finfo)==TRUE) {
				if(finfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY == 0) {
				// add this entry if it is NOT directory!
					tmp=new char[strlen(finfo.cFileName)+1];
					strcpy(tmp,finfo.cFileName);
					files.Add(tmp);
					tmp = new char[10];	// 1400000KB
					snprintf(tmp, 9, "%dKB", 
						((finfo.nFileSizeHigh * (MAXDWORD+1)) + finfo.nFileSizeLow));
					sizes.Add(tmp);	// add this size to list
				}
			} /* while */ 
		} /* if */ 

#endif
// do sorting for files
			if(files.Length() > 2 && B < files.Length())
			{
				N = files.Length() - 1;
//				B = 1;
				for(i = N; i > B; i--)
					for(j = B; j < i; j++)
						if(strcasecmp(files[j], files[j + 1]) > 0)
						{
							files.Swap(j,j + 1);
							sizes.Swap(j,j + 1);
						}
			}


//	Count out cursor position and file number output
	act_file = *index_file;
	if(act_file >= files.Length()) act_file = 0;		// cannot be more than files in list
	first_file = act_file - (FILES_IN_SCREEN / 2);
	if (first_file < 0) first_file = 0;	// cannot be negativ...

// Show all directories (first) and files then
//	char *tmp;
	char *siz;
//	int i;

// prepare screen
		double facx = double(g_ScreenWidth) / double(SCREEN_WIDTH);
		double facy = double(g_ScreenHeight) / double(SCREEN_HEIGHT);

	SDL_Surface *tempSurface = NULL;
	if(!g_WindowResized) {
		if(g_nAppMode == MODE_LOGO) tempSurface = g_hLogoBitmap;	// use logobitmap
			else tempSurface = g_hDeviceBitmap;
	}
	else tempSurface = g_origscreen;

	if(tempSurface == NULL) 
		tempSurface = screen;	// use screen, if none available
		
	my_screen = SDL_CreateRGBSurface(SDL_SWSURFACE, tempSurface->w, tempSurface->h, tempSurface->format->BitsPerPixel, 0, 0, 0, 0);
	if(tempSurface->format->palette && my_screen->format->palette)
		SDL_SetColors(my_screen, tempSurface->format->palette->colors,
			      0, tempSurface->format->palette->ncolors);

	surface_fader(my_screen, 0.2F, 0.2F, 0.2F, -1, 0);	// fade it out to 20% of normal
	SDL_BlitSurface(tempSurface, NULL, my_screen, NULL);

	while(true)
	{

		SDL_BlitSurface(my_screen, NULL, screen, NULL);		// show background

		font_print_centered(sx/2 ,5*facy , incoming_dir, screen, 1.5*facx, 1.3*facy);
		if (slot == 6) font_print_centered(sx/2,20*facy,"Choose image for floppy 140KB drive", screen, 1*facx, 1*facy);
		else
			if (slot == 7) font_print_centered(sx/2,20*facy,"Choose image for Hard Disk", screen, 1*facx, 1*facy);
		else
			if (slot == 5) font_print_centered(sx/2,20*facy,"Choose image for floppy 800KB drive", screen, 1*facx, 1*facy);
		else
			if (slot == 1) font_print_centered(sx/2,20*facy,"Select file name for saving snapshot", screen, 1*facx, 1*facy);
		else
			if (slot == 0) font_print_centered(sx/2,20*facy,"Select snapshot file name for loading", screen, 1*facx, 1*facy);

		font_print_centered(sx/2,30*facy, "Press ENTER to choose, or ESC to cancel",screen, 1.4*facx, 1.1*facy);

		files.Rewind();	// from start
		sizes.Rewind();
		i = 0;

//		printf("We've printed some messages, go to file list!\n");
// show all fetched dirs and files
// topX of first fiel visible
		int TOPX	= int(45*facy);

		while(files.Iterate(tmp)) {

			sizes.Iterate(siz);	// also fetch size string

			if (i >= first_file && i < first_file + FILES_IN_SCREEN)
			{ // FILES_IN_SCREEN items on screen
//				char tmp2[80],tmp3[256];

				if (i == act_file) { // show item under cursor (in inverse mode)
					SDL_Rect r;
					r.x= 2;
					r.y= TOPX + (i-first_file) * 15 * facy - 1;
					if(strlen(tmp) > 46) r.w = 46 * 6 * 1.7 * facx + 2;
					   else r.w= strlen(tmp) * 6 * 1.7 * facx + 2;	// 6- FONT_SIZE_X
					r.h= 9 * 1.5 * facy;
					SDL_FillRect(screen, &r, SDL_MapRGB(screen->format,255,0,0));// in RED
				} /* if */

				// print file name with enlarged font
				char ch;
				ch = 0;
				if(strlen(tmp) > 46) { ch = tmp[46]; tmp[46] = 0;} //cut-off too long string
				font_print(4, TOPX + (i - first_file) * 15 * facy, tmp, screen, 1.7*facx, 1.5*facy); // show name
				font_print(sx - 70 * facx, TOPX + (i - first_file) * 15 * facy, siz, screen, 1.7*facx, 1.5*facy);// show info (dir or size)
				if(ch) tmp[46] = ch; //restore cut-off char

			} /* if */
			i++;		// next item
		} /* while */

/////////////////////////////////////////////////////////////////////////////////////////////
// draw rectangles
	rectangle(screen, 0, TOPX - 5, sx, 320*facy, SDL_MapRGB(screen->format, 255, 255, 255));
	rectangle(screen, 480*facx, TOPX - 5, 0, 320*facy, SDL_MapRGB(screen->format, 255, 255, 255));

	SDL_Flip(screen);	// show the screen
	SDL_Delay(KEY_DELAY);	// wait some time to be not too fast

	//////////////////////////////////
	// Wait for keypress
	//////////////////////////////////
	SDL_Event event;	// event
	Uint8 *keyboard;	// key state

	event.type = SDL_QUIT;
	while(event.type != SDL_KEYDOWN) {	// wait for key pressed
				SDL_Delay(10);
				SDL_PollEvent(&event);
	}

// control cursor
		keyboard = SDL_GetKeyState(NULL);	// get current state of pressed (and not pressed) keys
		if (keyboard[SDLK_UP] || keyboard[SDLK_LEFT]) {
			if (act_file>0) act_file--;	// up one position
			if (act_file<first_file) first_file=act_file;
		} /* if */

		if (keyboard[SDLK_DOWN] || keyboard[SDLK_RIGHT]) {
			if (act_file < (files.Length() - 1)) act_file++;
			if (act_file >= (first_file + FILES_IN_SCREEN)) first_file=act_file - FILES_IN_SCREEN + 1;
		} /* if */

		if (keyboard[SDLK_PAGEUP]) {
			act_file-=FILES_IN_SCREEN;
			if (act_file<0) act_file=0;
			if (act_file<first_file) first_file=act_file;
		} /* if */

		if (keyboard[SDLK_PAGEDOWN]) {
			act_file+=FILES_IN_SCREEN;
			if (act_file>=files.Length()) act_file=(files.Length()-1);
			if (act_file>=(first_file+FILES_IN_SCREEN)) first_file=act_file-FILES_IN_SCREEN + 1;
		} /* if */

		// choose an item?
		if (keyboard[SDLK_RETURN]) {
			// dup string from selected file name
			*filename = strdup(files[act_file]);
//			printf("files[act_file]=%s, *filename=%s\n\n", files[act_file], *filename);
			if(!strcmp(sizes[act_file], "<DIR>") || !strcmp(sizes[act_file], "<UP>"))
			   		*isdir = true;
				else *isdir = false;	// this is directory (catalog in Apple][ terminology)
			*index_file = act_file;	// remember current index
			files.Delete();
			sizes.Delete();
			SDL_FreeSurface(my_screen);

			return true;
		} /* if */

		if (keyboard[SDLK_ESCAPE]) {
			files.Delete();
			sizes.Delete();
			SDL_FreeSurface(my_screen);

			return false;		// ESC has been pressed
		} /* if */

		if (keyboard[SDLK_HOME]) {	// HOME?
			act_file=0;
			first_file=0;
		} /* if */
		if (keyboard[SDLK_END]) {	// END?
			act_file=files.Length() - 1;	// go to the last possible file in list
			first_file=act_file - FILES_IN_SCREEN + 1;
			if(first_file < 0) first_file = 0;
		} /* if */

	}
} /* ChooseAnImage */

#pragma once

#include <Windows.h>			// for Windows API functions
#include <stdio.h>
#include <unistd.h>			// for read
#include <stdlib.h>			// for atexit
#include <ctype.h>			// for testing input ctrl characters
#include <signal.h>			// for supressing termination signals
#include <errno.h>
#include <string.h>

/********* DEFINITIONS *********/

#define CTRL_KEY(k) ((k) & 0x1f)		// allows for processing ctrl inputs
#define ABUF_INIT {NULL, 0}

// stores terminal configurations
typedef struct terminalConfig {
	
	int cx, cy;					// cursor x/y
	DWORD terminal;				// console settings	
} termconfig;

struct terminalConfig dflt;		// terminal defaults
struct terminalConfig edit;		// for handing editor processing

// append buffer for writing
typedef struct appendbuffer{				
	char* b;
	int len;
} abuf;

/**** FUNCTION DECLARATION ****/

void die(const char *s);
void disableRawMode();			// returns to console defaults
void enableRawMode();			// turn off echoing etc.
char readKey();
void processKey();
void clrScr();					// clears all text execept for title
void rfrScr();					// updates screen to changes
void abAppend(abuf* ab, const char* s, int len);
void abFree(abuf* ab);
int getCursorPosition(int *rows, int *cols);
void moveCrsr(char key);
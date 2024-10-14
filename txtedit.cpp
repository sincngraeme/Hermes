#include "txtedit.hh"

// prints errors and kills program
void die(const char *s)
{
	clrScr();
	perror(s);
	exit(1);
}

// function to reset terminal attributes

void disableRawMode()
{
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);			// Get console input handle
	
	if(hIn == INVALID_HANDLE_VALUE){ die("ERRROR: GetStdHandle"); }	// error checking
	
	SetConsoleMode(hIn, dflt.terminal);                     // set to terminal defaults
}

// function to set terminal attributes

void enableRawMode()
{
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);			// Get console input handle
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);		// Get console output handle
	
	if(hIn == INVALID_HANDLE_VALUE || hOut == INVALID_HANDLE_VALUE)		// error checking
	{
		die("ERROR: GetStdHandle");
	}

	GetConsoleMode(hIn, &dflt.terminal);			// store console defaults

	atexit(disableRawMode);					// register reset function to run when program terminates
	
	DWORD dwMode = 0;					// variable to store mode

	hIn = GetStdHandle(STD_INPUT_HANDLE);		
	GetConsoleMode(hIn, &dwMode);				// Get current mode

	// OFF
	SetConsoleMode(hIn, dwMode & ~(	ENABLE_ECHO_INPUT 		| 		// Disable echoed input
									ENABLE_LINE_INPUT 		|		// Disable canonical mode
									ENABLE_PROCESSED_INPUT 	));		// Diable ctrl
	// ON
	SetConsoleMode(hIn, dwMode & (	ENABLE_PROCESSED_OUTPUT));		// Enable processed output

	// console is now raw ;)	

	return;
}

// read keypress from user
char readKey()
{
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    if(hIn == INVALID_HANDLE_VALUE)
    {
        perror("ERROR getting input handle");
        return EOF;
    }
	
}

// process user input
void processKey()
{
	char c = readKey();

	switch (c)
	{
		case CTRL_KEY('q'): 							// quit cmd
			clrScr();
			exit(0);
			break;

		// case CTRL_KEY('t'): 							// test
			
		// 	break;

		case 'w':
		case 'a':
		case 's':
		case 'd':
			moveCrsr(c);
			break;

		case EOF:									// triggered by ctrl-z and so ignore
			break;

		// default:
		// 	if(!iscntrl(c)) putchar(c);				// TEMP !!!		
	}
}
void clrScr()
{
	printf("\033[%d;%dH", dflt.cy, dflt.cx);		// move cursor to editor 0,0
	printf("\033[0J", 4);							// ANSI escape sequence for clearing the screen from cursor
}
// function to clear screen using ANSI escape sequences
void rfrScr()
{
	abuf ab = ABUF_INIT;			// initialize buffer

  	abAppend(&ab, "\033[?25l", 6);		// set cursor to invisible

	char buf[32];

	snprintf(buf, sizeof(buf), "\033[%d;%dH", edit.cy, edit.cx);		// move cursor to x/y
	abAppend(&ab, buf, strlen(buf));
	//abAppend(&ab, "\033[0J", 4);		// ANSI escape sequence for clearing the screen from cursor
	abAppend(&ab, "\033[?25h", 6);		// reset cursor to visible

	write(STDOUT_FILENO, ab.b, ab.len);
	//printf("%s", ab.b);
	abFree(&ab);
}

// function for create/append to abuf -> MUST BE USED IN CONJUNCTION WITH abFree
void abAppend(abuf* ab, const char* s, int slen)
{
	char* new = realloc(ab->b, ab->len + slen);			// realloc functions as malloc here for new

	if(new == NULL) return;

	memcpy(&new[ab->len], s, slen); 					// copy s to end of new
	ab->b = new;										// pass new ptr to b
	ab->len += slen;									// record new size
}
// function to free abuf
void abFree(abuf* ab)
{
	free(ab->b);
	ab->len = 0;		// since mem has been freed the length is now 0
}

// gets the current cursor position
int getCursorPosition(int *rows, int *cols)
{
	char buf[32];
	unsigned int i = 0;

	if (write(STDOUT_FILENO, "\033[6n", 4) != 4) return -1;

	while (i < sizeof(buf) - 1) 
	{
		if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
		if (buf[i] == 'R') break;
		i++;
	}

	buf[i] = '\0';

	if (buf[0] != '\033' || buf[1] != '[') return -1;
	if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;

	return 0;
}
// move the cursor
void moveCrsr(char key)
{
	switch(key)
	{
		case 'w':
			edit.cy--;
			break;
		case 'a':
			edit.cx--;
			break;
		case 's':
			edit.cy++;
			break;
		case 'd':
			edit.cx++;
			break;
	}
}

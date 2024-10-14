/*
     _    _                               __  __       _                         
    | |  | |                             |  \/  |     (_)                        
    | |__| | ___ _ __ _ __ ___   ___  ___| \  / | __ _ _ _ __    ___ _ __  _ __  
    |  __  |/ _ \ '__| '_ ` _ \ / _ \/ __| |\/| |/ _` | | '_ \  / __| '_ \| '_ \ 
    | |  | |  __/ |  | | | | | |  __/\__ \ |  | | (_| | | | | || (__| |_) | |_) |
    |_|  |_|\___|_|  |_| |_| |_|\___||___/_|  |_|\__,_|_|_| |_(_)___| .__/| .__/ 
                                                                    | |   | |    
                                                                    |_|   |_|    

    A Command Line Integrated Development Environment for C/C++ Programming 
*/

#include "txtedit.hh"

int main()
{
printf("Hermes:\n\n");

	enableRawMode();		// set console attributes

	if(getCursorPosition(&edit.cy, &edit.cx) == -1) die("ERROR: getCursorPosition");		// store editor cursor position
	if(getCursorPosition(&dflt.cy, &dflt.cx) == -1) die("ERROR: getCursorPosition");		// set default cursor position (editor dflt not terminal dflt)

	while (1)
	{
		rfrScr();
		processKey();
	}

	return 0;
}
#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdio.h>

#ifdef _WIN32
#    include <windows.h>
#else
#    include <termios.h>
#    include <unistd.h>
#endif

#define RESET     "\033[0m"
#define BOLD      "\033[1m"
#define DIM       "\033[2m"
#define ITALIC    "\033[3m"
#define UNDERLINE "\033[4m"

#define BLACK   "\033[30m" 
#define RED     "\033[31m" 
#define GREEN   "\033[32m" 
#define YELLOW  "\033[33m" 
#define BLUE    "\033[34m" 
#define MAGENTA "\033[35m" 
#define CYAN    "\033[36m" 
#define WHITE   "\033[37m" 

#define BOLDBLACK   "\033[1m\033[30m"
#define BOLDRED     "\033[1m\033[31m"
#define BOLDGREEN   "\033[1m\033[32m"
#define BOLDYELLOW  "\033[1m\033[33m"
#define BOLDBLUE    "\033[1m\033[34m"
#define BOLDMAGENTA "\033[1m\033[35m"
#define BOLDCYAN    "\033[1m\033[36m"
#define BOLDWHITE   "\033[1m\033[37m"

#define PROGRAM_COLOR BOLDBLUE
#define ARGUMENT_COLOR BOLDMAGENTA
#define TEXT_COLOR ""

#define FPATH CYAN "%s" RESET
#define ARGF ARGUMENT_COLOR "%s" RESET

void init_terminal() {
#if _WIN32
	SetConsoleOutputCP(CP_UTF8);  // Set output code page to UTF-8
	SetConsoleCP(CP_UTF8);        // Set input code page to UTF-8
#endif
	printf(RESET);
}

void clear_terminal() {
	printf("\033[H\033[J");
}

void remove_current_terminal_line() {
	printf("\033[A\033[K");
}

void set_cursor_position(int row, int col) {
	printf("\033[%d;%dH", row, col);
}

void get_cursor_position(int* row, int* col) {
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(hStdout, &csbi)) {
		*row = csbi.dwCursorPosition.Y;
		*col = csbi.dwCursorPosition.X;
	}
#else
	struct termios oldt, newt;
	char buf[32];
	unsigned int i = 0;

	// Save terminal settings and switch to raw mode
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	// Request cursor position
	write(STDOUT_FILENO, "\033[6n", 4);

	// Read response
	while (i < sizeof(buf) - 1) {
		if (read(STDIN_FILENO, buf + i, 1) != 1)
			break;
		if (buf[i] == 'R')
			break;
		i++;
	}
	buf[i] = '\0';

	// Restore terminal settings
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	// Parse response (ESC [ rows ; cols R)
	int r = *row;
	int c = *col;
	if (sscanf(buf, "\033[%d;%dR", &r, &c) == 2) {
		*row = r;
		*col = c;
	}
#endif
}

#endif // TERMINAL_H

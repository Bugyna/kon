#define NCURSES_WIDECHAR 1
#include <curses.h>
#include <locale.h>
#include <wchar.h>
#include "util.h"
#include "buffer.h"
#include "input.h"

void kon_init() {
	// setlocale(LC_ALL, "");
	initscr();
	raw();
	noecho();
	
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	keypad(stdscr, TRUE);
	attron(COLOR_PAIR(1));
}



int main() {
	BUFFER buffer;
	buffer_init_scratch(&buffer);
	kon_init();
	wint_t c;

	
	while (running){
		// printw("dwad");
		// c = getch();
		// c = get_wch();
		get_wch(&c);
		// mvaddch(0, 0, c);
		handle_input(&buffer, c);
		
	}

	endwin();

	return 0;

}


#pragma once
#define NCURSES_WIDECHAR 1
#include <ncurses.h>
#include <wchar.h>
#include "util.h"
#include "buffer.h"
#include "global.h"



#define REPEAT(fn, n, ...) for (int i = 0; i < n; i++) fn(...);


void delete_key(BUFFER* b, const wint_t c)
{
	wchar_t ch = L' ';
	if (b->c->x - 1 < 0) ch = L' ';
	else ch = b->line->text[b->c->x-1];

	line_delete(b, b->line, b->c, ch);
	
	move(b->c->ry, b->c->rx);
	// if (ch != L'\t')
		addch(' ');
	// move(b->c->ry, b->c->rx);
}


void add_key(BUFFER* b, const wint_t c)
{
	// mvadd_wch(0, 0, c);
	// mvwadd_wch(stdscr, 0, 0, c);
	line_add(b, b->line, b->c, c);
	// mvaddwstr(b->c->ry, b->c->rx-1, b->line->text+b->c->x-1);
	render_line(b);
	// mvaddwstr(b->c->ry, b->c->rx, b->line->text);
}


void handle_input(BUFFER* b, const wint_t c)
{
	wchar_t ch;
	switch(c) {
		case 27: case CTRL('w'):
			running = false;
			break;

		// case CTRL('s'):
			// save_file(&buffer, "test.txt");
			// break;
			
		// case KEY_HOME:
			// column_index = 0;
			// buffer.index = prev_line(&buffer, -1)+1;
			// move(row_index, column_index);
			// break;
			
		// case KEY_END:
			// buffer.index += next_line(&buffer, -1);
			// column_index = next_line(&buffer, -1);
			// move(row_index, column_index);
			// break;

					
		case KEY_RIGHT:
			move_right(b);
			break;
			
		case KEY_LEFT:
			move_left(b);
		break;
			
		case KEY_DOWN:
			move_down(b);
			break;

		case KEY_UP:
			move_up(b);
			break;
		
		case KEY_BACKSPACE: case 127:
			delete_key(b, c);
			break;


		case L'\t':
			line_add(b, b->line, b->c, L'\t');
			b->c->rx += 2;
			move(b->c->ry, b->c->rx);
			// mvaddwstr(b->c->ry, b->c->rx, L"  ");
			// mvaddwstr(b->c->ry, 0, L"dddddddddd");
			break;
																																									
		case 10: // Enter
			// insert_char(&buffer, '\n');
			// line_add(b->line, b->c, c);
			// buffer_create_new_line(b);
			move_down(b);
			break;
			
		default:
			add_key(b, c);
			break;
	}
}
#pragma once

#include <ncurses.h>
#include "util.h"

#include "hashmap.h"


typedef struct
{
	int x, y;
	int rx, ry;
	char* token;

} CURSOR;

typedef struct
{
	CURSOR start, end;
} MARK;

DEFINE_HASHMAP(CURSOR_MAP, CURSOR)

typedef struct
{
	wchar_t* text;
	int size, index, percieved;
} LINE;



typedef struct
{
	LINE* lines;
	LINE* line;
	int size, index;
	CURSOR* c;
	// CURSOR_MAP cursors;
	CURSOR* cursors;

} BUFFER;



void dbg(BUFFER* b, const char* s)
{
	move(21, 0);
	clrtoeol();
	move(21, 0);
	printw("dwawd");
	mvchgat(b->c->ry, b->c->rx, 1, A_REVERSE | A_BLINK | COLOR_PAIR(2), 1, NULL);
}

void cursor_init(CURSOR* c)
{
	c->x = 0;
	c->y = 0;
	c->rx = 0;
	c->ry = 0;
	c->token = NULL;
}

void line_init(LINE* l)
{
	l->index = 0;
	l->size = 20;
	l->text = calloc(l->size, sizeof(wchar_t));
}

void buffer_init_scratch(BUFFER* b)
{
	b->size = 20;
	b->index = 10;
	b->lines = calloc(b->size, sizeof(LINE));
	// for (int i = 0; i < b->size; i++) {
		// line_init(&b->lines[i]);
	// }

	b->line = &b->lines[0];
	line_init(b->line);
	b->cursors = calloc(10, sizeof(CURSOR));
	b->c = &b->cursors[0];
	cursor_init(b->c);
	
}

void buffer_init_file(BUFFER* b, char* filename)
{
	
}

void buffer_create_new_line(BUFFER* b)
{
	// b->lines = realloc(b->lines, (b->size++)*sizeof(LINE));
	if (b->index+1 >= b->size || b->c->y+1 > b->size) {
		b->size += 10;
		b->lines = realloc(b->lines, b->size*sizeof(LINE));
	}

	if (b->lines[b->c->y+1].text == NULL || b->lines[b->c->y+1].text[0] == L'\0') return;

	b->index++;
	memmove(&b->lines[b->c->y+2], &b->lines[b->c->y+1], b->index-b->c->y);
	b->line = &b->lines[b->c->y];
	
	line_init(b->line);
}

void line_add(BUFFER* b, LINE* l, CURSOR* c, wchar_t ch)
{
	if (l->index + 1 >= l->size)
	{
		l->size += 20;
		l->text = realloc(l->text, l->size*sizeof(wchar_t));
	}

	if (ch == L'\t') l->percieved += 3;
	else l->percieved++;

	if (c->x == l->index) {
		l->text[l->index++] = ch;
		l->text[l->index] = L'\0';
		c->x++;
		c->rx++;
		return;
	}

	// memmove(&l->text+(c->x+1), &l->text[c->x], l->index-c->x+1);
	l->text[c->x] = ch;
	l->index++;

	c->x++;
	c->rx++;
}

void move_down(BUFFER* b)
{
	if (b->c->y+1 > b->size) return;

	b->c->ry++;
	b->c->y++;
	b->line = &b->lines[b->c->y];

	if (b->c->x > b->line->index || b->c->rx > b->line->percieved) {
		b->c->x = b->line->index;
		b->c->rx = b->line->percieved;
	}

	// move(b->c->ry, b->c->rx);
}

void move_up(BUFFER* b)
{
	b->c->ry--;
	b->c->y--;
	b->line = &b->lines[b->c->y];

	if (b->c->x > b->line->index || b->c->rx > b->line->percieved) {
		b->c->x = b->line->index;
		b->c->rx = b->line->percieved;
	}

	
	// move(b->c->ry, b->c->rx);
}

void move_up_eol(BUFFER* b)
{
	if (b->c->y-1 < 0) return;
	b->c->ry--;
	b->c->y--;
	b->line = &b->lines[b->c->y];

	b->c->x = b->line->index;
	b->c->rx = b->line->percieved;
	
	// move(b->c->ry, b->c->rx);
}

void move_left(BUFFER* b)
{
	if (b->c->x - 1 < 0) {
		move_up_eol(b);
		return;
	}

	wchar_t ch = b->line->text[b->c->x-1];

	if (ch == L'\t') b->c->rx -= 3;
	else b->c->rx--;
	
	b->c->x--;
	// move(b->c->ry, b->c->rx);
}

void move_right(BUFFER* b)
{
	if (b->c->x + 1 > b->line->index) {
		move_down(b);
		return;
	}
	b->c->rx++;
	b->c->x++;
}

void line_delete(BUFFER* b, LINE* l, CURSOR* c, wchar_t ch)
{
	if (c->x-1 < 0) {
		// dbg(b, "huih");
		move_up_eol(b);
		return;
	}
	memmove(&l->text+(c->x-1), &l->text[c->x], l->index-c->x);
	c->x--;
	l->index--;

	if (ch == L'\t') {
		c->rx-=3;
		l->percieved -= 3;
	}
	else {
		l->percieved--;
		c->rx--;
	}

	// else if (ch == L'\n')
	// {
		// move_up_eol(b);
	// }
}


void render_line(BUFFER* b)
{
	move(b->c->ry, 0);
	int col = 0;
	for (int i = 0; i < b->line->index; i++) {
		if (b->line->text[i] == L'\t') { col += 3; continue; }
		mvaddnwstr(b->c->ry, col++, b->line->text+i, 1);
	}
}

void clear_cursor(BUFFER* b)
{
	mvchgat(b->c->ry, b->c->rx, 1, A_NORMAL, 0, NULL);
}



void render_cursor(BUFFER* b)
{
	move(20, 0);
	clrtoeol();
	move(20, 0);
	printw("%d.%d | %d.%d", b->c->ry, b->c->rx, b->c->y, b->c->x);
	mvchgat(b->c->ry, b->c->rx, 1, WA_REVERSE, 0, NULL);
	// mvaddstr(20, b->c->rx, "222");
}





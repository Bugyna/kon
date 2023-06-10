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
	int size, index;
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
	b->size = 10;
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
	if (b->lines[b->c->y+1].text == NULL) return;

	b->index++;
	memmove(&b->lines[b->c->y+2], &b->lines[b->c->y+1], b->index-b->c->y);
	b->line = &b->lines[b->c->y];
	// b->line = calloc(1, sizeof(LINE));
	line_init(b->line);
}

void line_add(BUFFER* b, LINE* l, CURSOR* c, wchar_t ch)
{
	if (l->index + 1 >= l->size)
	{
		l->size += 20;
		l->text = realloc(l->text, l->size*sizeof(wchar_t));
	}

	if (c->x == l->index) {
		l->text[l->index++] = ch;
		l->text[l->index] = L'\0';
		c->x++;
		c->rx++;
		return;
	}

	memmove(&l->text+(c->x+1), &l->text[c->x], l->index-c->x+1);
	l->text[c->x] = ch;
	l->index++;
	c->x++;
	c->rx++;
}

void move_down(BUFFER* b, int n)
{
	b->c->ry++;
	b->c->y++;
	b->c->x = 0;
	b->c->rx = 0;
	b->line = &b->lines[b->c->y];
	move(b->c->ry, b->c->rx);
}

void move_up(BUFFER* b, int n)
{
	b->c->ry--;
	b->c->y--;
	b->line = &b->lines[b->c->y];

	b->c->x = 0;
	b->c->rx = 0;
	
	move(b->c->ry, b->c->rx);
}

void move_up_eol(BUFFER* b, int n)
{
	if (b->c->y == 0) return;
	b->c->ry--;
	b->c->y--;
	b->line = &b->lines[b->c->y];

	b->c->x = b->line->index;
	b->c->rx = b->line->index;
	
	move(b->c->ry, b->c->rx);
}

void line_delete(BUFFER* b, LINE* l, CURSOR* c, wchar_t ch)
{
	if (l->index == 0) {
		move_up_eol(b, 1);
		return;
	}
	memmove(&l->text+(c->x-1), &l->text[c->x], l->index-c->x);
	l->index--;
	c->x--;
	if (ch == L'\t') c->rx-=3;
	else if (ch == L'\n')
	{
		move_up_eol(b, 1);
	}
	else c->rx--;
}


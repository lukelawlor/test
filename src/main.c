/*
 * main.c contains the main function and game loop
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ncurses.h>

#include "main.h"
#include "win.h"
#include "entity/entity.h"
#include "map.h"
#include "error.h"

// Game area
char *area;

// Game hour (1-24)
int hour;

// Game minute (0-59)
int min;

int main(void)
{
	// Init ncurses
	if (initscr() == NULL)
	{
		PERR("failed to initialize ncurses");
		exit(EXIT_FAILURE);
	}

	if (noecho() == ERR)
	{
		PERR("ncurses noecho() call failed");
		exit(EXIT_FAILURE);
	}

	if (curs_set(0) == ERR)
		PERR("cursor invisibility not supported by terminal");

	// Init game windows
	
	// End stdscr so error messages aren't buried
	endwin();
	if (init_windows() != 0)
		exit(EXIT_FAILURE);
	
	// Bring stdscr back
	refresh();

	// Set global variables
	area = "Nowhere";
	hour = 6;
	min = 0;
	
	// Set map data to create a basic map with nothing but air and a rectangular wall border
	for (int y = 0; y < MAPH; y++)
	{
		for (int x = 0; x < MAPW; x++)
		{
			if (y == 0 || x == 0 || y == MAPH - 1 || x == MAPW - 1)
				MAPT(y, x) = MAPTILE_WALL;
			else
				MAPT(y, x) = MAPTILE_AIR;
		}
	}

	// Create a player entity and place it in the world
	new_entity(1, 1, "Player", 'p', player_update);
	new_entity(4, 4, "Goblin", 'g', goblin_update);
	new_entity(4, 4, "Goblin", 'g', goblin_update);
	new_entity(4, 4, "Goblin", 'g', goblin_update);
	new_entity(4, 4, "Goblin", 'g', goblin_update);
	new_entity(4, 4, "Goblin", 'g', goblin_update);
	new_entity(4, 4, "Goblin", 'g', goblin_update);
	
	// Draw the entire map
	draw_map();

	// Draw infowin
	draw_infowin();
	wrefresh(infowin);

	// Printing test text
	waddstr(statwin, "statwin");
	wrefresh(statwin);
	waddstr(talkwin, "talkwin");
	wrefresh(talkwin);

	// Game loop
	for (;;)
	{
		// Update entities
		for (ELNode *node = &elhead; node->e != NULL; node = node->next)
		{
			(*node->e->update)(node->e);
		}
	}

	// Game end
	endwin();
	exit(EXIT_SUCCESS);
}

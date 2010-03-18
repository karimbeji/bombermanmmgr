#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
#include "graphics.h"

int main (int argc, char * argv[])
{
	int finished = 0;
	maze_t * maze;
	
	maze = loadMaze (argv[1]);		// Chargement du plateau du jeu.
	initWindow (maze->w, maze->h);	// Fenêtre SDL.
	loadTiles ();					// Chargement des cases du jeu.

	while (!finished)
	{
		finished = getEvent ();
		paint (maze);				// Mise à jour des cases.
	}
	
	unloadMaze (maze);				// Libération de la mémoire.

	return 0;
}


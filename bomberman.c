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
	initWindow (maze->w, maze->h);	// Fen�tre SDL.
	loadTiles ();					// Chargement des cases du jeu.
	
	while (!finished)
	{
		finished = getEvent (maze);	// R�cup�ration des �v�nements.
		updatePlayer(maze, 0);			// Mise � jour des joueurs.
		updateBomb(maze);			// Mise � jour des bombes.
		SDL_Delay (150);			// Ralentissmeent.
		// printf("Rand : %d\n", (rand() % 5));
		paint (maze);				// Mise � jour des cases.
	}
	
	unloadMaze (maze);				// Lib�ration de la m�moire.

	return 0;
}


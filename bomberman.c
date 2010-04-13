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
		finished = getEvent (maze);	// Récupération des évènements.
		updatePlayer(maze, 0);			// Mise à jour des joueurs.
		updateBomb(maze);			// Mise à jour des bombes.
		SDL_Delay (150);			// Ralentissmeent.
		// printf("Rand : %d\n", (rand() % 5));
		paint (maze);				// Mise à jour des cases.
	}
	
	unloadMaze (maze);				// Libération de la mémoire.

	return 0;
}


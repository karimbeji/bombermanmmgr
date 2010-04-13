#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
#include "graphics.h"

// Fin du jeu.
int endGame = 0;

int main (int argc, char * argv[])
{
	int i, finished = 0, nbPlayerAlive = 4;
	maze_t * maze;
	
	maze = loadMaze (argv[1]);		// Chargement du plateau du jeu.
	initWindow (maze->w, maze->h);	// Fen�tre SDL.
	loadTiles ();					// Chargement des cases du jeu.
	
	while (!finished)
	{
		finished = getEvent (maze);	// R�cup�ration des �v�nements.
		updatePlayer(maze, 0);						// Mise � jour des joueurs.
		updateBomb(maze);							// Mise � jour des bombes.
		SDL_Delay (150);							// Ralentissmeent.
		paint (maze);								// Mise � jour des cases.
		nbPlayerAlive = lastPlayer();				// V�rifie que les joueurs soit vivant
		
		if (endGame == 0 && nbPlayerAlive == 1)
		{
			for (i = 0; i < 4; i++)
			{
				if (arrayPlayer[i].alive == 1)
				{
					break;
				}
			}
			printf("^_^ Le jouer %d vient de gagner la partie !!! ^_^\n", i); // Affichage du vainqueur.
			endGame = 1;
		}
		else if (endGame == 0 && nbPlayerAlive == 0)
		{
			printf("^_^ Tout le monde a perdu !!! ^_^\n");
			endGame = 1;
		}
	}
	
	unloadMaze (maze);				// Lib�ration de la m�moire.

	return 0;
}


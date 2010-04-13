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
	initWindow (maze->w, maze->h);	// Fenêtre SDL.
	loadTiles ();					// Chargement des cases du jeu.
	
	while (!finished)
	{
		finished = getEvent (maze);	// Récupération des évènements.
		updatePlayer(maze, 0);						// Mise à jour des joueurs.
		updateBomb(maze);							// Mise à jour des bombes.
		SDL_Delay (150);							// Ralentissmeent.
		paint (maze);								// Mise à jour des cases.
		nbPlayerAlive = lastPlayer();				// Vérifie que les joueurs soit vivant
		
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
	
	unloadMaze (maze);				// Libération de la mémoire.

	return 0;
}


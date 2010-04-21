#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
#include "graphics.h"
#include "network.h"

int main (int argc, char * argv[])
{
	// Pour �viter les segmentation fault.
	if (argc > 1)
	{
		int finished, weHaveOneWinner;

		finished = weHaveOneWinner = 0;
		
		// Initialisation et affectation du plateau de jeu.
		maze_t * maze = (maze_t *) malloc (sizeof (maze_t));
	
		// On charge la carte.
		loadMaze (argv[1], maze);
		// Affichage de la fen�tre SDL.
		initWindow (maze->w, maze->h);
		// Chargement des images du jeu.
		loadTiles ();
	
		// D�but du jeu.
		while (!finished)
		{
			// R�cup�ration des �v�nements.
			getEvent (maze, &finished);
			// Mise � jour de la position des joueurs.
			updatePlayer (maze, 1);
			// Mise � jour des explosions.
			updateExplosion (maze);
			// Mise � jour des bombes.
			updateBomb (maze);
			// On ralentit un peu le jeu.
			SDL_Delay (175);
			// Mise � jour des cases.
			paint (maze);
			// on v�rifie que la partie n'est pas termin�e.
			updateOutput (&weHaveOneWinner);
		}
	
		// Lib�ration de la m�moire.
		unloadMaze (maze);
	}
	else
		printf ("Veuillez renseigner un plateau de jeu.\n./bomberman maze\n");

	return 0;
}
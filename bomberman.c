#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
#include "graphics.h"
#include "network.h"

int main (int argc, char * argv[])
{
	// Pour éviter les segmentation fault.
	if (argc > 1)
	{
		int finished, weHaveOneWinner;

		finished = weHaveOneWinner = 0;
		
		// Initialisation et affectation du plateau de jeu.
		maze_t * maze = (maze_t *) malloc (sizeof (maze_t));
	
		// On charge la carte.
		loadMaze (argv[1], maze);
		// Affichage de la fenêtre SDL.
		initWindow (maze->w, maze->h);
		// Chargement des images du jeu.
		loadTiles ();
	
		// Début du jeu.
		while (!finished)
		{
			// Récupération des évènements.
			getEvent (maze, &finished);
			// Mise à jour de la position des joueurs.
			updatePlayer (maze, 1);
			// Mise à jour des explosions.
			updateExplosion (maze);
			// Mise à jour des bombes.
			updateBomb (maze);
			// On ralentit un peu le jeu.
			SDL_Delay (175);
			// Mise à jour des cases.
			paint (maze);
			// on vérifie que la partie n'est pas terminée.
			updateOutput (&weHaveOneWinner);
		}
	
		// Libération de la mémoire.
		unloadMaze (maze);
	}
	else
		printf ("Veuillez renseigner un plateau de jeu.\n./bomberman maze\n");

	return 0;
}
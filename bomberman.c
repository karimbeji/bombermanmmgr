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
		int finished = 0;

		// Initialisation et affectation du plateau de jeu.
		maze_t * maze = (maze_t *) malloc (sizeof (maze_t));
	
		// Initialisation de la connexion client.
		// connection_t * socket = initClient ("127.0.0.1", 7000);
	
		// On charge la carte.
		loadMaze (argv[1], maze);
		// Affichage de la fenêtre SDL.
		initWindow (maze->w, maze->h);
		// Chargement des images du jeu.
		loadTiles ();
	
		// Début du jeu.
		while (!finished)
		{
		// 	receiveData (socket, maze, sizeof (tile_t) * maze->w * maze->h);	// On reçoit la map.
		// 	receiveData (socket, arrayPlayer, sizeof(player_t) * 4);			// On reçoit les joueurs.
			// Récupération des évènements.
			getEvent (maze, &finished);
			// Mise à jour de la position des joueurs.
			updatePlayer (maze, 0);
			// Mise à jour des explosions.
			updateExplosion (maze);
			// Mise à jour des bombes.
			updateBomb (maze);
			// On ralentit un peu le jeu.
			SDL_Delay (175);
			// Mise à jour des cases.
			paint (maze);
		// 	broadcastData (socket, maze, sizeof (tile_t) * maze->w * maze->h);	// On envoie la map.
		// 	broadcastData (socket, arrayPlayer, sizeof(player_t) * 4);			// On envoie les joueurs.
		}
	
		// Fermeture socket.
		// closeClient (socket);
	
		// Libération de la mémoire.
		unloadMaze (maze);
	}
	else
		printf ("Veuillez renseigner un plateau de jeu.\n./bomberman maze.txt\n");

	return 0;
}
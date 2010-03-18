#include <stdio.h>
#include <stdlib.h>
#include "maze.h"

/* Charge un fichier dont on donne le nom et retourne une structure
maze_t correspondant au plateau de jeu. Le fichier doit
correspondre au format donné par l'énoncé.
Si le fichier ne peut pas être lu, retourne NULL. */
maze_t * loadMaze (char *filename)
{
	char buffer[1024];
	int i,j;
	maze_t * maze;

	// Ouvre le fichier en lecture.
	FILE * f = fopen (filename, "r");
	
	if (f == NULL) 
	{
		return NULL;
	}

	// Alloue de l'espace pour le plateau de jeu.
	maze = (maze_t *) malloc (sizeof (maze_t));

	// Lit les dimensions du plateau de jeu. w correspond au nombre de colonnes, h au nombre de lignes.
	fscanf (f, "%d %d\n", &maze->w, &maze->h);
	fprintf (stderr, "%d %d\n", maze->w, maze->h);

	// Alloue les tuiles constituant le plateau de jeu.
	maze->t = (tile_t *) malloc (sizeof (tile_t) * maze->w * maze->h);
	
	// Lit le fichier ligne par ligne et construit les tuiles du plateau.
	for (i = 0; i < maze->h; i++)
	{
		// Lit une ligne.
		fgets (buffer, 1024, f);
		// Buffer est un tableau de caractère. Pour chaque caractère, construit les tuiles.
		for (j = 0; j < maze->w; j++)
		{
			switch (buffer[j])
			{
				default:
				case '.':
					maze->t[j * maze->h + i].type = EMPTY;
					break;
				case '+':
					maze->t[j * maze->h + i].type = SOFTWALL;
					break;
				case '#':
					maze->t[j * maze->h + i].type = HARDWALL;
					break;
			}
		}
	}
	
	fclose (f);
	
	return maze;
}

// Désalloue le plateau de jeu.
void unloadMaze (maze_t * maze)
{
	free (maze->t);
	free (maze);
}
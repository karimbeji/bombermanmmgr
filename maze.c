#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
#include "time.h"

/* Charge un fichier dont on donne le nom et retourne une structure
maze_t correspondant au plateau de jeu. Le fichier doit
correspondre au format donné par l'énoncé.
Si le fichier ne peut pas être lu, retourne NULL. */
maze_t * loadMaze (char * filename)
{
	char buffer[1024];
	int i, j, nbPlayer, bonus;
	maze_t * maze;

	// Ouvre le fichier en lecture.
	FILE * f = fopen (filename, "r");
	
	if (f == NULL) 
	{
		return NULL;
	}

	// Alloue de l'espace pour le plateau de jeu.
	maze = (maze_t *) malloc (sizeof (maze_t));
	// Alloue de l'espace pour le tableau de joueur.
	arrayPlayer = malloc(sizeof(player_t) * 4);

	// Lit les dimensions du plateau de jeu. w correspond au nombre de colonnes, h au nombre de lignes.
	fscanf (f, "%d %d\n", &maze->w, &maze->h);
	fprintf (stderr, "%d %d\n", maze->w, maze->h);

	// Alloue les tuiles constituant le plateau de jeu.
	maze->t = (tile_t *) malloc (sizeof (tile_t) * maze->w * maze->h);
	
	// Lit le fichier ligne par ligne et construit les tuiles du plateau.
	for (i = 0, nbPlayer = 0; i < maze->h; i++)
	{
		// Lit une ligne.
		fgets (buffer, 1024, f);
		// Buffer est un tableau de caractère. Pour chaque caractère, construit les tuiles.
		for (j = 0; j < maze->w; j++)
		{
			// Par défaut.
			maze->t[i * maze->h + j].bonus = 0;

			switch (buffer[j])
			{
				default:
				case '.':
					maze->t[i * maze->h + j].type = T_EMPTY;
					break;
				case '+':
					maze->t[i * maze->h + j].type = T_SOFTWALL;
					
					// Génération du bonus.
					bonus = generateBonus (3); // 33% de chance d'avoir un bonus...

					if (bonus == 0) // On génèreles bonus.
					{
						maze->t[i * maze->h + j].bonus = 1;
					}
					break;
				case '#':
					maze->t[i * maze->h + j].type = T_HARDWALL;
					break;
				case '@':
					maze->t[i * maze->h + j].type = T_EMPTY;
					
					if (nbPlayer < 4)
					{
						arrayPlayer[nbPlayer].alive = 1;				// Si le joueur est vivant.
						arrayPlayer[nbPlayer].powerBomb = 1; 			// Puissance des bombe par défaut.
						arrayPlayer[nbPlayer].direction = STOP; 		// Direction du joueur.
						arrayPlayer[nbPlayer].x = j; 					// Affectation des coordonnés.
						arrayPlayer[nbPlayer].y = i; 					// Affectation des coordonnés.

						nbPlayer++;
					}
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

// Vérifie si un joueur n'est pas sur la case suivante.
int checkOtherPlayer(int numPlayer, enum direction_e dir)
{
	int i, j;
	j = 0;
	
	for (i = 0; i < 4; i++) // Pour tous les joueurs.
	{
		if (i != numPlayer && arrayPlayer[i].alive == 1)
			switch (dir)
			{
				case TOP:
					j = (arrayPlayer[i].y == (arrayPlayer[numPlayer].y - 1) && arrayPlayer[i].x == arrayPlayer[numPlayer].x) ? j + 1 : j;
					break;
				case BOTTOM:
					j = (arrayPlayer[i].y == (arrayPlayer[numPlayer].y + 1) && arrayPlayer[i].x == arrayPlayer[numPlayer].x) ? j + 1 : j;
					break;
				case LEFT:
					j = (arrayPlayer[i].y == arrayPlayer[numPlayer].y && arrayPlayer[i].x == (arrayPlayer[numPlayer].x - 1)) ? j + 1 : j;
					break;
				case RIGHT:
					j = (arrayPlayer[i].y == arrayPlayer[numPlayer].y && arrayPlayer[i].x == (arrayPlayer[numPlayer].x + 1)) ? j + 1 : j;
					break;
				default: ;
			}
	}

	return j;
}

// Vérifie si le type de la case convient à un déplacement.
int checkTileOK (enum tile_e nextCase)
{
	if (nextCase == T_EMPTY || nextCase == T_BONUS || nextCase == T_EXPLOSION)
		return 0;
	else
		return 1;
}

// Génération aléatoire des bombes sous les murs destructibles...
int generateBonus (int lucky)
{
	static int j = 69; // Chiffre qui initialise la fonction random...
	srand ((time (NULL) * j++)); // Petite cuisine aléatoire !

	return (rand() % lucky);
}

// Permet d'imprimer sur la sortie un peu de texte...
void updateOutput (int player, int alive, int power)
{
	if (alive == 0)
	{
		printf("Le joueur %d vient de mourir avec une puissance de %d !\n", player, power);
	}
	else if (power > 0)
	{
		printf("Le joueur %d vient d'augmenter sa puissance a %d !\n", player, power);
	}
}

// Permet de connaître le nom du vainqueur.
int lastPlayer ()
{
	int i, nbPlayerAlive;
	
	for (i = 0, nbPlayerAlive = 0; i < 4; i++)
	{
		if (arrayPlayer[i].alive == 1)
		{
			nbPlayerAlive++;
		}
	}
	
	return nbPlayerAlive;
}
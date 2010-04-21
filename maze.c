#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
#include "time.h"

// Vérifie que 2 joueurs ne se retrouvent pas sur la même case.
int checkOtherPlayer(int numPlayer)
{
	int iPlayer, result = 0;
	
	for (iPlayer = 0; iPlayer < nbPlayerDefault; iPlayer++)
	{
		if (iPlayer != numPlayer && arrayPlayer[iPlayer].alive == 1)
		{
			switch (arrayPlayer[numPlayer].direction)
			{
				case UP:
					result = (arrayPlayer[iPlayer].y == (arrayPlayer[numPlayer].y - 1) && arrayPlayer[iPlayer].x == arrayPlayer[numPlayer].x) ? result + 1 : result;
					break;
				case DOWN:
					result = (arrayPlayer[iPlayer].y == (arrayPlayer[numPlayer].y + 1) && arrayPlayer[iPlayer].x == arrayPlayer[numPlayer].x) ? result + 1 : result;
					break;
				case LEFT:
					result = (arrayPlayer[iPlayer].y == arrayPlayer[numPlayer].y && arrayPlayer[iPlayer].x == (arrayPlayer[numPlayer].x - 1)) ? result + 1 : result;
					break;
				case RIGHT:
					result = (arrayPlayer[iPlayer].y == arrayPlayer[numPlayer].y && arrayPlayer[iPlayer].x == (arrayPlayer[numPlayer].x + 1)) ? result + 1 : result;
					break;
				default: ;
			}
		}
	}

	return result;
}

// Vérifie que le type de la case soit autorisé.
int checkTileOK (enum tile_e nextCase)
{
	if (nextCase == T_EMPTY || nextCase == T_BONUS || nextCase == T_EXPLOSION)
		return 0;
	else
		return 1;
}

// Explosion d'une bombe.
void explosion (maze_t * maze, int numTile)
{
	int arrayStop[4], iPower, iDirection, iPlayer; // Tableau stopant l'explosion pour un mur indestructible, variable dela boucle.
	explosion_s arrayExplosion[4];
	
	// Initialisation du tableau qui permettra de connaître les limites de l'explosion.
	arrayStop[0] = arrayStop[1] = arrayStop[2] = arrayStop[3] = 0;

	// Explosion de la case contenant la bombe.
	maze->t[numTile].type = T_EXPLOSION;
	maze->t[numTile].timer = 0;
	
	for (iPlayer = 0; iPlayer <= nbPlayerDefault; iPlayer++)
	{
		if (linearTile (maze, arrayPlayer[iPlayer].x, arrayPlayer[iPlayer].y) == numTile)
		{
			arrayPlayer[iPlayer].alive = 0;
		}
	}

	// En fonction de la puissance du joueur.
	for (iPower = 1; iPower <= maze->t[numTile].power; iPower++)
	{
		// Affectation du tableau des direction qui permet d'éviter de réécrire du code...
		arrayExplosion[0].test = ((maze->w * iPower) <= numTile);
		arrayExplosion[1].test = ((numTile % maze->w) < (maze->w - iPower));
		arrayExplosion[2].test = (numTile < (maze->w * (maze->h - iPower)));
		arrayExplosion[3].test = (iPower <= (numTile % maze->w));
		arrayExplosion[0].tile = (numTile - maze->w * iPower);
		arrayExplosion[1].tile = (numTile + iPower);
		arrayExplosion[2].tile = (numTile + maze->w * iPower);
		arrayExplosion[3].tile = (numTile - iPower);

		// Boucle des directions.
		for (iDirection = 0; iDirection < 4; iDirection++)
		{
			if (arrayExplosion[iDirection].test && arrayStop[iDirection] == 0)
			{
				switch (maze->t[arrayExplosion[iDirection].tile].type)
				{
					case T_HARDWALL:
						arrayStop[iDirection] = 1;
						break;
					case T_BOMB:
						explosion (maze, arrayExplosion[iDirection].tile);
						break;
					default:
						maze->t[arrayExplosion[iDirection].tile].type = T_EXPLOSION;
						maze->t[arrayExplosion[iDirection].tile].timer = 0;
						
						for (iPlayer = 0; iPlayer <= nbPlayerDefault; iPlayer++)
						{
							if (linearTile (maze, arrayPlayer[iPlayer].x, arrayPlayer[iPlayer].y) == arrayExplosion[iDirection].tile)
							{
								arrayPlayer[iPlayer].alive = 0;
							}
						}
						break;
				}
			}
		}
	}
}

// Génération aléatoire des bombes sous les murs destructibles.
int generateBonus (int lucky)
{
	static int j = 69; // Chiffre qui initialise la fonction random...
	srand ((time (NULL) * j++)); // Petite cuisine aléatoire !

	return (rand() % lucky);
}

// Calcule la coordonnée linéaire à l'aide des coordonnées X et Y du plateau.
int linearTile (maze_t * maze, int x, int y)
{
	return (y * maze->w + x);
}

// Charge le plateau de jeu à partir d'un fichier texte.
void loadMaze (char * filename, maze_t * maze)
{
	char buffer[1024], filename2[64];
	int line, row, nbPlayer, tempTile, lucky;
	
	// Concatene le nom de fichier.
	sprintf (filename2, "map/%s.txt", filename);
	printf ("%s\n", filename2);
	// Ouvre le fichier en lecture.
	FILE * file = fopen (filename2, "r");
	
	if (file == NULL) 
	{
		maze = NULL;
	}

	// Lit les dimensions du plateau de jeu. w correspond au nombre de colonnes, h au nombre de lignes.
	fscanf (file, "%d %d %d %d\n", &maze->w, &maze->h, &nbPlayerDefault, &lucky);

	// Alloue de l'espace pour le tableau de joueur.
	arrayPlayer = malloc(sizeof(player_t) * nbPlayerDefault);

	// Information pour le joeur.
	fprintf (stderr, "Lancement du jeu...\nLargeur : %d, hauteur : %d, nombre de joueurs : %d.\n", maze->w, maze->h, nbPlayerDefault);

	// Alloue les cases constituant le plateau de jeu.
	maze->t = (tile_t *) malloc (sizeof (tile_t) * maze->w * maze->h);
	
	// Lit le fichier ligne par ligne et construit les tuiles du plateau.
	for (line = 0, nbPlayer = 0; line < maze->h; line++)
	{
		// Lit une ligne.
		fgets (buffer, 1024, file);

		// Buffer est un tableau de caractère. Pour chaque caractère, construit les tuiles.
		for (row = 0; row < maze->w; row++)
		{
			// Coordonnée linéaire de la case courante.
			tempTile = linearTile (maze, row, line);
			// Par défaut.
			maze->t[tempTile].bonus = 0;
			maze->t[tempTile].power = 0;
			maze->t[tempTile].timer = 0;

			switch (buffer[row])
			{
				default:
				case '.':
					maze->t[tempTile].type = T_EMPTY;
					break;
				case '+':
					maze->t[tempTile].type = T_SOFTWALL;
					// Génération aléatoire des bonus.
					if (generateBonus ((lucky > 0 ? lucky : 3)) == 0) maze->t[tempTile].bonus = 1;
					break;
				case '#':
					maze->t[tempTile].type = T_HARDWALL;
					break;
				case '@':
					maze->t[tempTile].type = T_EMPTY;
					
					if (nbPlayerDefault == 0 || nbPlayer < nbPlayerDefault)
					{
						arrayPlayer[nbPlayer].alive = 1;				// Si le joueur est vivant.
						arrayPlayer[nbPlayer].powerBomb = 1; 			// Puissance des bombe par défaut.
						arrayPlayer[nbPlayer].direction = STOP; 		// Direction du joueur.
						arrayPlayer[nbPlayer].x = row; 					// Affectation des coordonnés.
						arrayPlayer[nbPlayer].y = line; 					// Affectation des coordonnés.

						nbPlayer++;
					}
					break;
			}
		}
	}
	
	fclose (file);
}

// Déplacement du joueur.
void movePlayer (maze_t * maze, int numPlayer)
{
 	int nextTile = nextTileType (maze, numPlayer);

	if (checkTileOK(nextTile) == 0 && checkOtherPlayer(numPlayer) == 0)
	{
		if (arrayPlayer[numPlayer].direction == UP)
			arrayPlayer[numPlayer].y -= 1;
		else if (arrayPlayer[numPlayer].direction == RIGHT)
			arrayPlayer[numPlayer].x += 1;
		else if (arrayPlayer[numPlayer].direction == DOWN)
			arrayPlayer[numPlayer].y += 1;
		else if (arrayPlayer[numPlayer].direction == LEFT)
			arrayPlayer[numPlayer].x -= 1;
		else
			arrayPlayer[numPlayer].direction = STOP;
	}
	else
		arrayPlayer[numPlayer].direction = STOP;
}

// Retourne le type de la prochaine case du joueur.
int nextTileType (maze_t * maze, int numPlayer)
{
 	int currentPlace = linearTile (maze, arrayPlayer[numPlayer].x, arrayPlayer[numPlayer].y);

	switch (arrayPlayer[numPlayer].direction)
	{
		case UP:
			return (arrayPlayer[numPlayer].y > 0 ? maze->t[(currentPlace - maze->w)].type : T_HARDWALL);
			break;
		case RIGHT:
			return (arrayPlayer[numPlayer].x < (maze->w - 1) ? maze->t[(currentPlace + 1)].type : T_HARDWALL);
			break;
		case DOWN:
			return (arrayPlayer[numPlayer].y < (maze->h - 1) ? maze->t[(currentPlace + maze->w)].type : T_HARDWALL);
			break;
		case LEFT:
			return (arrayPlayer[numPlayer].x > 0 ? maze->t[(currentPlace - 1)].type : T_HARDWALL);
			break;
		default: 
			return T_HARDWALL;
			break;
	}
}

// Désalloue le plateau de jeu.
void unloadMaze (maze_t * maze)
{
	free (maze->t);
	free (maze);
	free (arrayPlayer);
}

// Met à jour les bombes.
void updateBomb (maze_t * maze)
{
	int iTile; // Variable de boucle (case courante).

	for (iTile = 0; iTile < (maze->w * maze->h); iTile++) // Boucle pour les bombes.
	{
		if (maze->t[iTile].type == T_BOMB)
		{
			if (maze->t[iTile].timer == 0)
				explosion (maze, iTile);
			else
				maze->t[iTile].timer -= 1;
		}
	}
}

// Met à jour les les explosions.
void updateExplosion (maze_t * maze)
{
	int iTile;

	// Boucle pour les explosions.
	for (iTile = 0; iTile < (maze->w * maze->h); iTile++)
	{
		if (maze->t[iTile].type == T_EXPLOSION)
		{
			if (maze->t[iTile].timer >= 1)
			{
				if (maze->t[iTile].bonus == 1)
				{
					maze->t[iTile].type = T_BONUS;
					maze->t[iTile].bonus = 0;
				}
				else
					maze->t[iTile].type = T_EMPTY;
				
				maze->t[iTile].power = 0;
				maze->t[iTile].timer = 0;
			}
			else
				maze->t[iTile].timer += 1;
		}
	}
}

// Permet d'afficher un peu de texte dans la console.
void updateOutput (int * endGame)
{
	if ((*endGame) == 0)
	{
		int iPlayer, nbPlayerAlive , playerWinner;

		for (iPlayer = 0, nbPlayerAlive = 0; iPlayer < nbPlayerDefault; iPlayer++)
		{
			nbPlayerAlive += arrayPlayer[iPlayer].alive;
			playerWinner = iPlayer;
		}

		if (nbPlayerAlive == 1)
		{
			printf ("\n^_^   ^_-   Le joueur %d a gagne la partie !   -_^   ^_^\n\n", playerWinner);
			(*endGame) = 1;
		}
	}
}

// Met à jour la position du joueur.
void updatePlayer (maze_t * maze, int stepByStep)
{
	int iPlayer, tilePlayer; // Variable de boucle (joueur courant), case courante du joueur.

	// Gestion des déplacements des joueurs.
	for (iPlayer = 0; iPlayer < nbPlayerDefault; iPlayer++)
	{
		if (arrayPlayer[iPlayer].alive == 1 && arrayPlayer[iPlayer].direction != STOP)
		{	
			movePlayer (maze, iPlayer);
			
			// Mise à jour de la position du joueur.
			tilePlayer = linearTile (maze, arrayPlayer[iPlayer].x, arrayPlayer[iPlayer].y);
		
			if (maze->t[tilePlayer].type == T_EXPLOSION) // Permet de tuer un joueur.
			{
				arrayPlayer[iPlayer].alive = 0;
			}
			else if (maze->t[tilePlayer].type == T_BONUS) // permet de ramasser des bonus.
			{
				arrayPlayer[iPlayer].powerBomb += 1;
				maze->t[tilePlayer].type = T_EMPTY;
			}
	
	 		// Permet de faire du pas à pas.
			if (stepByStep == 1) arrayPlayer[iPlayer].direction = STOP;
		}
	}
}
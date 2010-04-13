#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "maze.h"
#include "graphics.h"

// Fenetre de jeu.
SDL_Surface * screen = NULL;

// Numéro des différentes tuiles.
enum tile_number {
	EMPTY = 0,
	HARDWALL,
	SOFTWALL,
	BOMB,
	POWER,
	FIRE,
	PLAYER1,
	PLAYER2,
	PLAYER3,
	PLAYER4,
	DEAD,
	MAX_TILE_NUMBER
};

// Taille d'une image (carrée).
#define SIZE 32
// Timer bomb.
#define TIMER_BOMB 10

// Les images des différentes tuiles possibles du jeu.
SDL_Surface * tile[MAX_TILE_NUMBER];

const char * tilenames[]= {
        NULL, // Pas d'image quand la case est vide.
        "data/hardwall.bmp",
        "data/softwall.bmp",
        "data/bomb.bmp",
        "data/power.bmp",
        "data/fire.bmp",
        "data/player1.bmp",
        "data/player2.bmp",
        "data/player3.bmp",
        "data/player4.bmp",
        "data/dead.bmp"
};

player_t joueur[4];

// Charge les différentes images du jeu...
void loadTiles ()
{
	int i;
	
	for (i = 0; i < MAX_TILE_NUMBER; i++)
	{
		tile[i] = SDL_LoadBMP (tilenames[i]);
	}
}

// Affiche tout le plateau de jeu.
void paint (maze_t * maze)
{
	int nbPlayer, w, h, z;
	SDL_Rect rect;
	rect.w = SIZE;
	rect.h = SIZE;

	nbPlayer = 0;

	// Efface l'écran en le mettant tout noir.
	SDL_FillRect (screen, NULL, SDL_MapRGB (screen->format, 0, 0, 0));
	
	for (h = 0; h < maze->h; h++)
	{
		for (w = 0; w < maze->w; w++)		
		{
			rect.x = SIZE * w;
			rect.y = SIZE * h;

			switch (maze->t[h * maze->w + w].type)
			{
				default:
				case T_EMPTY:
					SDL_BlitSurface(tile[EMPTY], NULL, screen, &rect);
					break;
				case T_HARDWALL:
					SDL_BlitSurface(tile[HARDWALL], NULL, screen, &rect);
					break;
				case T_SOFTWALL:
					SDL_BlitSurface(tile[SOFTWALL], NULL, screen, &rect);
					break;
				case T_BOMB:
					SDL_BlitSurface(tile[BOMB], NULL, screen, &rect);
					break;
				case T_EXPLOSION:
					SDL_BlitSurface(tile[FIRE], NULL, screen, &rect);
					break;
				case T_BONUS:
					SDL_BlitSurface(tile[POWER], NULL, screen, &rect);
					break;
			}
		}
	}
	
	// Affichage des morts !
	for (z = 0; z < 4; z++)
	{
		rect.x = SIZE * arrayPlayer[z].x;
		rect.y = SIZE * arrayPlayer[z].y;
		
		if (arrayPlayer[z].alive == 0)
		{
			SDL_BlitSurface(tile[DEAD], NULL, screen, &rect);
		}
	}
	
	// Affichage des vivants.
	for (z = 0; z < 4; z++)
	{
		rect.x = SIZE * arrayPlayer[z].x;
		rect.y = SIZE * arrayPlayer[z].y;
		
		if (arrayPlayer[z].alive == 1)
		{
			SDL_BlitSurface(tile[PLAYER1 + z], NULL, screen, &rect);
		}
	}

	// Met a jour la fenetre.
	SDL_Flip(screen);
}

// Capture les evenements clavier/fenetre.
int getEvent (maze_t * maze)
{
	SDL_Event event;
	// Ecoute les événements qui sont arrivés.

	while (SDL_PollEvent (&event))
	{
		// On a fermé la fenetre -> quitter le jeu.
		if (event.type == SDL_QUIT)
		{
			return 1;
		}

		// On a appuyé sur une touche.
		if (event.type == SDL_KEYDOWN)
		{
 			switch (event.key.keysym.sym)
			{
				// Si c'est la touche escape, quitter le jeu.
 				case SDLK_ESCAPE:
					return 1;
					break;
				case SDLK_SPACE:
					if (arrayPlayer[0].alive == 1)
					{
						maze->t[arrayPlayer[0].y * maze->w + arrayPlayer[0].x].type = T_BOMB;
						maze->t[arrayPlayer[0].y * maze->w + arrayPlayer[0].x].power = arrayPlayer[0].powerBomb;
						maze->t[arrayPlayer[0].y * maze->w + arrayPlayer[0].x].timer = TIMER_BOMB;
					}
					break;
				case SDLK_LEFT:
					arrayPlayer[0].direction = LEFT;
					break;
				case SDLK_RIGHT:
					arrayPlayer[0].direction = RIGHT;
					break;
				case SDLK_UP:
					arrayPlayer[0].direction = TOP;
					break;
				case SDLK_DOWN:
					arrayPlayer[0].direction = BOTTOM;
					break;
				case SDLK_a:
					if (arrayPlayer[1].alive == 1)
					{
						maze->t[arrayPlayer[1].y * maze->w + arrayPlayer[1].x].type = T_BOMB;
						maze->t[arrayPlayer[1].y * maze->w + arrayPlayer[1].x].power = arrayPlayer[1].powerBomb;
						maze->t[arrayPlayer[1].y * maze->w + arrayPlayer[1].x].timer = TIMER_BOMB;
					}
					break;
				case SDLK_q:
					arrayPlayer[1].direction = LEFT;
					break;
				case SDLK_d:
					arrayPlayer[1].direction = RIGHT;
					break;
				case SDLK_z:
					arrayPlayer[1].direction = TOP;
					break;
				case SDLK_s:
					arrayPlayer[1].direction = BOTTOM;
					break;
				default: ;
			}
		}
	}

	return 0;
}

// Met à jour la position du joueur.
void updatePlayer (maze_t * maze, int stepByStep)
{
	int player, currentPlace, nextPlace; // Variable de boucle (joueur courant), case courante du joueur.
	enum tile_e nextCaseTop, nextCaseBottom, nextCaseLeft, nextCaseRight; // Variable de la prochaine case.

	for (player = 0; player < 4; player++) // Gestion des déplacements des joueurs.
	{
		if (arrayPlayer[player].alive == 1)
		{
			currentPlace = (arrayPlayer[player].y * maze->w + arrayPlayer[player].x);
			nextCaseTop = arrayPlayer[player].y > 0 ? maze->t[(currentPlace - maze->w)].type : T_HARDWALL;
			nextCaseBottom = arrayPlayer[player].y < (maze->h - 1) ? maze->t[(currentPlace + maze->w)].type : T_HARDWALL;
			nextCaseLeft = arrayPlayer[player].x > 0 ? maze->t[(currentPlace - 1)].type : T_HARDWALL;
			nextCaseRight = arrayPlayer[player].x < (maze->w - 1) ? maze->t[(currentPlace + 1)].type : T_HARDWALL;

			switch (arrayPlayer[player].direction)
			{
				// Déplacement vers le haut.
				case TOP:
					// On vérifie la limite de la map, les obstacles et les joueurs de la case suivante.
					if (checkTileOK(nextCaseTop) == 0 && checkOtherPlayer(player, TOP) == 0)
						arrayPlayer[player].y -= 1;
					else
						arrayPlayer[player].direction = STOP;
					break;
				// Déplacement vers le droite.
				case RIGHT:
					// On vérifie la limite de la map, les obstacles et les joueurs de la case suivante.
					if (checkTileOK(nextCaseRight) == 0 && checkOtherPlayer(player, RIGHT) == 0)
						arrayPlayer[player].x += 1;
					else
						arrayPlayer[player].direction = STOP;
					break;
				// Déplacement vers le bas.
				case BOTTOM:
					// On vérifie la limite de la map, les obstacles et les joueurs de la case suivante.
					if (checkTileOK(nextCaseBottom) == 0 && checkOtherPlayer(player, BOTTOM) == 0)
						arrayPlayer[player].y += 1;
					else
						arrayPlayer[player].direction = STOP;
					break;
				// Déplacement vers le gauche.
				case LEFT:
					// On vérifie la limite de la map, les obstacles et les joueurs de la case suivante.
					if (checkTileOK(nextCaseLeft) == 0 && checkOtherPlayer(player, LEFT) == 0)
						arrayPlayer[player].x -= 1;
					else
						arrayPlayer[player].direction = STOP;
					break;
				default: ;
			}
		
			// Mise à jour de la position du joueur.
			nextPlace = (arrayPlayer[player].y * maze->w + arrayPlayer[player].x);
		
			if (maze->t[nextPlace].type == T_EXPLOSION) // Permet de tuer un joueur.
			{
				arrayPlayer[player].alive = 0;

				updateOutput (player, 0, arrayPlayer[player].powerBomb);
			}
			else if (maze->t[nextPlace].type == T_BONUS) // permet de ramasser des bonus.
			{
				arrayPlayer[player].powerBomb += 1;
				maze->t[nextPlace].type = T_EMPTY;

				updateOutput (player, 1, arrayPlayer[player].powerBomb);
			}
		
			if (stepByStep == 1) // Permet de faire du pas à pas.
			{
				arrayPlayer[player].direction = STOP;
			}
		}
	}
}

// Met à jour les bombes et les explosions.
void updateBomb (maze_t * maze)
{
	int varPlace; // Variable de boucle (case courante).

	for (varPlace = 0; varPlace < (maze->w * maze->h); varPlace++) // Boucle pour les bombes.
	{
		if (maze->t[varPlace].type == T_BOMB)
		{
			if (maze->t[varPlace].timer == 0)
			{
				explosion (maze, varPlace);
			}
			else
			{
				maze->t[varPlace].timer -= 1;
			}
		}
	}

	for (varPlace = 0; varPlace < (maze->w * maze->h); varPlace++) // Boucle pour les explosions.
	{
		if (maze->t[varPlace].type == T_EXPLOSION)
		{
			if (maze->t[varPlace].timer == 1)
			{
				if (maze->t[varPlace].bonus == 1)
				{
					maze->t[varPlace].type = T_BONUS;
					maze->t[varPlace].bonus = 0;
				}
				else
				{
					maze->t[varPlace].type = T_EMPTY;
				}
			}
			else
			{
				maze->t[varPlace].timer += 1;
			}
		}
	}	

	/* Obligation : Je suis obligé de faire 2 boucles sinon j'ai un décalage dans les explosions et je n'arriva pas à trouver une meilleure solution algorithmique pour résoudre ce dernier !!! */
}

// Explosion d'une bombe.
void explosion (maze_t * maze, int numTile)
{
	int arrayStop[4], i; // Tableau stopant l'explosion pour un mur indestructible, varaible de boucle.
	
	// Initialisation du tableau qui permettra de connaître les limites de l'explosion.
	arrayStop[0] = arrayStop[1] = arrayStop[2] = arrayStop[3] = 0;

	// Explosion de la case contenant la bombe.
	maze->t[numTile].type = T_EXPLOSION;

	for (i = 1; i <= maze->t[numTile].power; i++) // En fonction de la puissance du joueur.
	{		
		// Top
		if ((maze->w * i) <= numTile && arrayStop[0] == 0)
		{
			switch (maze->t[numTile - maze->w * i].type)
			{
				case T_HARDWALL:
					arrayStop[0] = 1;
					break;
				default:
					maze->t[numTile - maze->w * i].type = T_EXPLOSION;
					maze->t[numTile - maze->w * i].timer = 0;
					break;
			}
		}
		
		// Right
		if ((numTile % maze->w) < (maze->w - i) && arrayStop[1] == 0)
		{
			switch (maze->t[numTile + i].type)
			{
				case T_HARDWALL:
					arrayStop[1] = 1;
					break;
				default:
					maze->t[numTile + i].type = T_EXPLOSION;
					maze->t[numTile + i].timer = 0;
					break;
			}
		}
		
		// Bottom
		if (numTile < (maze->w * (maze->h - i)) && arrayStop[2] == 0)
		{
			switch (maze->t[numTile + maze->w * i].type)
			{
				case T_HARDWALL:
					arrayStop[2] = 1;
					break;
				default:
					maze->t[numTile + maze->w * i].type = T_EXPLOSION;
					maze->t[numTile + maze->w * i].timer = 0;
					break;
			}
		}
		
		// Left
		if (i <= (numTile % maze->w) && arrayStop[3] == 0)
		{
			switch (maze->t[numTile - i].type)
			{
				case T_HARDWALL:
					arrayStop[3] = 1;
					break;
				default:
					maze->t[numTile - i].type = T_EXPLOSION;
					maze->t[numTile - i].timer = 0;
					break;
			}
		}
	}
}

// Ouvre une fenêtre avec de taille wxh tuiles de plateau de jeu.
void initWindow (int w, int h)
{
	if (SDL_Init (SDL_INIT_VIDEO) == -1)
	{
		fprintf(stderr, "error %s\n", SDL_GetError ());
		exit(1);
	}

	atexit (SDL_Quit);
	fprintf (stderr, "size %d %d\n", w * SIZE, h * SIZE);
	screen = SDL_SetVideoMode (w * SIZE, h * SIZE, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);
	
	if (screen == NULL)
	{
		fprintf (stderr, "error video mode: %s\n", SDL_GetError ());
		exit(1);
	}
}
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
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
	"data/player4.bmp"
};

// Nom des joueurs.
const char * namesPlayer[] = {
	"duBoursin",
	"yChieDur",
	"yChieMou",
	"prendDuLaxatif"
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

// Affiche tout le plateau de jeu. A COMPLETER.
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
			}
		}
	}
	
	for (z = 0; z < 4; z++)
	{
		rect.x = SIZE * arrayPlayer[z].x;
		rect.y = SIZE * arrayPlayer[z].y;
		SDL_BlitSurface(tile[PLAYER1 + z], NULL, screen, &rect);
	}

	// Affiche les tuiles du plateau de jeu.
	// A REMPLIR.
	// Les images des tuiles sont à afficher avec:
//	SDL_SetColorKey (tile[0], SDL_SRCCOLORKEY, SDL_MapRGB (screen->format, 0, 0, 0));
//	SDL_BlitSurface(tile[0], NULL, screen, &rect);

	// Met a jour la fenetre.
	SDL_Flip(screen);
}

// Capture les evenements clavier/fenetre.
// Retourne 1 si il faut quitter le jeu, 0 sinon.*/
// A REMPLIR.
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
					maze->t[arrayPlayer[3].y * maze->w + arrayPlayer[3].x].type = T_BOMB;
					maze->t[arrayPlayer[3].y * maze->w + arrayPlayer[3].x].power = arrayPlayer[3].powerBomb;
					maze->t[arrayPlayer[3].y * maze->w + arrayPlayer[3].x].timer = TIMER_BOMB;
					break;
				case SDLK_LEFT:
					arrayPlayer[3].direction = LEFT;
					break;
				case SDLK_RIGHT:
					arrayPlayer[3].direction = RIGHT;
					break;
				case SDLK_UP:
					arrayPlayer[3].direction = TOP;
					break;
				case SDLK_DOWN:
					arrayPlayer[3].direction = BOTTOM;
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

void updatePaint (maze_t * maze)
{
	int numPlayer, currentPlace, varPlace; // Variable de boucle.
	enum tile_e nextCaseTop, nextCaseBottom, nextCaseLeft, nextCaseRight; // Variable de la prochaine case.

	for (numPlayer = 0; numPlayer < 4; numPlayer++) // Gestion des déplacements des joueurs.
	{
		currentPlace = (arrayPlayer[numPlayer].y * maze->w + arrayPlayer[numPlayer].x);
		nextCaseTop = arrayPlayer[numPlayer].y > 0 ? maze->t[(currentPlace - maze->w)].type : T_HARDWALL;
		nextCaseBottom = arrayPlayer[numPlayer].y < (maze->h - 1) ? maze->t[(currentPlace + maze->w)].type : T_HARDWALL;
		nextCaseLeft = arrayPlayer[numPlayer].x > 0 ? maze->t[(currentPlace - 1)].type : T_HARDWALL;
		nextCaseRight = arrayPlayer[numPlayer].x < (maze->w - 1) ? maze->t[(currentPlace + 1)].type : T_HARDWALL;

		switch (arrayPlayer[numPlayer].direction)
		{
			// Déplacement vers le haut.
			case TOP:
				// On vérifie la limite de la map, les obstacles et les joueurs de la case suivante.
				if ((nextCaseTop == T_EMPTY || nextCaseTop == T_BONUS) && checkOtherPlayer(numPlayer, TOP) == 0)
					arrayPlayer[numPlayer].y -= 1;
				else
					arrayPlayer[numPlayer].direction = STOP;
				break;
			// Déplacement vers le bas.
			case BOTTOM:
				// On vérifie la limite de la map, les obstacles et les joueurs de la case suivante.
				if ((nextCaseBottom == T_EMPTY || nextCaseBottom == T_BONUS) && checkOtherPlayer(numPlayer, BOTTOM) == 0)
					arrayPlayer[numPlayer].y += 1;
				else
					arrayPlayer[numPlayer].direction = STOP;
				break;
			// Déplacement vers le gauche.
			case LEFT:
				// On vérifie la limite de la map, les obstacles et les joueurs de la case suivante.
				if ((nextCaseLeft == T_EMPTY || nextCaseLeft == T_BONUS) && checkOtherPlayer(numPlayer, LEFT) == 0)
					arrayPlayer[numPlayer].x -= 1;
				else
					arrayPlayer[numPlayer].direction = STOP;
				break;
			// Déplacement vers le droite.
			case RIGHT:
				// On vérifie la limite de la map, les obstacles et les joueurs de la case suivante.
				if ((nextCaseRight == T_EMPTY || nextCaseRight == T_BONUS) && checkOtherPlayer(numPlayer, RIGHT) == 0)
					arrayPlayer[numPlayer].x += 1;
				else
					arrayPlayer[numPlayer].direction = STOP;
				break;
			default: ;
		}
	}
	
	for (varPlace = 0; varPlace < (maze->w * maze->h); varPlace++)
	{
		if (maze->t[varPlace].type == T_BOMB)
			if (maze->t[varPlace].timer == 0)
				maze->t[varPlace].type = T_EXPLOSION;
			else
				maze->t[varPlace].timer--;
		else if (maze->t[varPlace].type == T_EXPLOSION)
			maze->t[varPlace].type = T_EMPTY;
	}
}

// Ouvre une fenêtre avec de taille wxh tuiles de plateau de jeu.. 
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
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "maze.h"
#include "graphics.h"

// Taille d'une image (carrée).
#define SIZE 32
// Timer bomb.
#define TIMER_BOMB 5

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

// Capture les evenements clavier/fenetre.
void getEvent (maze_t * maze, int * finished)
{
	SDL_Event event;
	int id = 0; // Introduit pour le réseaux.

	// Ecoute les événements qui sont arrivés.
	while (SDL_PollEvent (&event))
	{
		// On a fermé la fenetre -> quitter le jeu.
		if (event.type == SDL_QUIT)
		{
			(* finished) = 1;
			break;
		}

		// On a appuyé sur une touche.
		if (event.type == SDL_KEYDOWN)
		{
 			switch (event.key.keysym.sym)
			{
				// Si c'est la touche escape, quitter le jeu.
 				case SDLK_ESCAPE:
					(* finished) = 1;
					break;
				// Joueur 0.
				case SDLK_SPACE:
					if (arrayPlayer[id].alive == 1)
					{
						maze->t[arrayPlayer[id].y * maze->w + arrayPlayer[id].x].type = T_BOMB;
						maze->t[arrayPlayer[id].y * maze->w + arrayPlayer[id].x].power = arrayPlayer[id].powerBomb;
						maze->t[arrayPlayer[id].y * maze->w + arrayPlayer[id].x].timer = (arrayPlayer[id].powerBomb >= TIMER_BOMB ? arrayPlayer[id].powerBomb +  2 : TIMER_BOMB);
						// Pour pouvoir échapper à ses bombes...
					}
					break;
				case SDLK_LEFT:
					arrayPlayer[id].direction = LEFT;
					break;
				case SDLK_RIGHT:
					arrayPlayer[id].direction = RIGHT;
					break;
				case SDLK_UP:
					arrayPlayer[id].direction = UP;
					break;
				case SDLK_DOWN:
					arrayPlayer[id].direction = DOWN;
					break;
				// Joueur 1.
				case SDLK_f:
					if (arrayPlayer[1].alive == 1)
					{
						maze->t[arrayPlayer[1].y * maze->w + arrayPlayer[1].x].type = T_BOMB;
						maze->t[arrayPlayer[1].y * maze->w + arrayPlayer[1].x].power = arrayPlayer[1].powerBomb;
						maze->t[arrayPlayer[1].y * maze->w + arrayPlayer[1].x].timer = (arrayPlayer[1].powerBomb >= TIMER_BOMB ? arrayPlayer[1].powerBomb +  2 : TIMER_BOMB);
						// Pour pouvoir échapper à ses bombes...
					}
					break;
				case SDLK_q:
					arrayPlayer[1].direction = LEFT;
					break;
				case SDLK_d:
					arrayPlayer[1].direction = RIGHT;
					break;
				case SDLK_z:
					arrayPlayer[1].direction = UP;
					break;
				case SDLK_s:
					arrayPlayer[1].direction = DOWN;
					break;
				default: ;
			}
		}
	}
}

// Ouvre une fenêtre SDL.
void initWindow (int w, int h)
{
	if (SDL_Init (SDL_INIT_VIDEO) == -1)
	{
		fprintf (stderr, "error %s\n", SDL_GetError ());
		exit (1);
	}

	atexit (SDL_Quit);
	fprintf (stderr, "Taille du plateau de jeu : %dpx x %dpx.\n", w * SIZE, h * SIZE);
	screen = SDL_SetVideoMode (w * SIZE, h * SIZE, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);
	
	if (screen == NULL)
	{
		fprintf (stderr, "error video mode: %s\n", SDL_GetError ());
		exit (1);
	}
	
	SDL_EnableKeyRepeat(25, 25);
}

// Charge les différentes images du jeu...
void loadTiles ()
{
	int i;
	
	for (i = 0; i < MAX_TILE_NUMBER; i++)
		tile[i] = SDL_LoadBMP (tilenames[i]);
}

// Affiche tout le plateau de jeu.
void paint (maze_t * maze)
{
	int w, h, iPlayer;
	SDL_Rect rect;
	rect.w = SIZE;
	rect.h = SIZE;

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
					SDL_BlitSurface (tile[EMPTY], NULL, screen, &rect);
					break;
				case T_HARDWALL:
					SDL_BlitSurface (tile[HARDWALL], NULL, screen, &rect);
					break;
				case T_SOFTWALL:
					SDL_BlitSurface (tile[SOFTWALL], NULL, screen, &rect);
					break;
				case T_BOMB:
					SDL_BlitSurface (tile[BOMB], NULL, screen, &rect);
					break;
				case T_EXPLOSION:
					SDL_BlitSurface (tile[FIRE], NULL, screen, &rect);
					break;
				case T_BONUS:
					SDL_BlitSurface (tile[POWER], NULL, screen, &rect);
					break;
			}
		}
	}

	// Affichage des joueurs vivants.
	for (iPlayer = 0; iPlayer < 4; iPlayer++)
	{
		rect.x = SIZE * arrayPlayer[iPlayer].x;
		rect.y = SIZE * arrayPlayer[iPlayer].y;
		
		if (arrayPlayer[iPlayer].alive == 1)
			SDL_BlitSurface(tile[PLAYER1 + iPlayer], NULL, screen, &rect);
	}

	// Met a jour la fenetre.
	SDL_Flip(screen);
}
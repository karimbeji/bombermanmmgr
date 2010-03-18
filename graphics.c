#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
#include "graphics.h"

/* Fenetre de jeu */
SDL_Surface *screen=NULL;

/* Numéro des différentes tuiles */
enum tile_number {
  EMPTY=0,
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
/* Taille d'une image (carrée) */
#define SIZE 32

/* Les images des différentes tuiles possibles du jeu */
SDL_Surface *tile[MAX_TILE_NUMBER];
const char *tilenames[]={
  NULL, /* Pas d'image quand la case est vide */
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

/* Charge les différentes images du jeu . */
void loadTiles() {
  int i;
  for (i=0; i<MAX_TILE_NUMBER; i++)  {
    tile[i]=SDL_LoadBMP(tilenames[i]);
  }
}

/* Affiche tout le plateau de jeu. A COMPLETER */
void paint(maze_t *m) {
  SDL_Rect rect;
  rect.w = SIZE;
  rect.h = SIZE;
  
  /* Efface l'écran en le mettant tout noir */
  SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));

  /* Affiche les tuiles du plateau de jeu */
  /* A REMPLIR */
  /* Les images des tuiles sont à afficher avec:
     SDL_SetColorKey(tile[...],SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,0,0,0));
     SDL_BlitSurface(tile[...],NULL,screen,&rect);
  */

  /* Met a jour la fenetre */
  SDL_Flip(screen);
}

/* Capture les evenements clavier/fenetre */
/* Retourne 1 si il faut quitter le jeu, 0 sinon.*/
/* A REMPLIR */
int getEvent() {
  SDL_Event event;
  /* Ecoute les événements qui sont arrivés */
  while( SDL_PollEvent(&event) ){
    /* On a fermé la fenetre -> quitter le jeu */
    if (event.type==SDL_QUIT) return 1;
    /* On a appuyé sur une touche */
    if (event.type==SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
	/* Si c'est la touche escape, quitter le jeu */
      case SDLK_ESCAPE: return 1;
	/* A REMPLIR */
      default: ;
      }
    }
  }
  return 0;
}

/* Ouvre une fenêtre avec de taille wxh tuiles de plateau de jeu. */ 
void initWindow(int w,int h) {
  if (SDL_Init(SDL_INIT_VIDEO)==-1) {
    fprintf(stderr,"error %s\n",SDL_GetError());
    exit(1);
  }
  atexit(SDL_Quit);
  fprintf(stderr,"size %d %d\n",w*SIZE,h*SIZE);
  screen = SDL_SetVideoMode(w*SIZE,h*SIZE,32,SDL_SWSURFACE|SDL_DOUBLEBUF);
  if (screen== NULL) {
    fprintf(stderr,"error video mode: %s\n", SDL_GetError()) ;
    exit(1);
  }
}

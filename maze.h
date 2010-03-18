/* Définit le type d'une case du plateau de jeu */
typedef struct {
  /* A REMPLIR */
} tile_t;


/* Définit le plateau de jeu */
typedef struct {
  int w,h;
  tile_t *t;
} maze_t;


/* Déclaration des fonctions concernant le moteur de jeu */
/* loadMaze: charge d'un fichier un nouveau plateau de jeu */
maze_t *loadMaze(char *filename);
/* unloadMaze: désalloue le plateau de jeu */
void unloadMaze(maze_t *maze);

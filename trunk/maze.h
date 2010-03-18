/* D�finit le type d'une case du plateau de jeu */
typedef struct {
  /* A REMPLIR */
} tile_t;


/* D�finit le plateau de jeu */
typedef struct {
  int w,h;
  tile_t *t;
} maze_t;


/* D�claration des fonctions concernant le moteur de jeu */
/* loadMaze: charge d'un fichier un nouveau plateau de jeu */
maze_t *loadMaze(char *filename);
/* unloadMaze: d�salloue le plateau de jeu */
void unloadMaze(maze_t *maze);

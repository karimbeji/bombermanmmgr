// Définition du type tile.
enum tile_e {
	EMPTY = 0, 	// Case vide.
	HARDWALL, 	// Mur indextructible.
	SOFTWALL, 	// Mur destructible.
	BOMB,		// Bombe.
	BONUS, 		// Bonus.
	EXPLOSION	// Explosion.
};

// Définit le type d'une case du plateau de jeu.
typedef struct {
	enum tile_e type; 	// Type de tile.
	int power;			// Puissance des bombes.
	int timer;			// Compte à rebour des bombes.
} tile_t;

// Définit le plateau de jeu.
typedef struct {
  int w,h;		// Largeur et Hauteur.
  tile_t *t;	// Tile.
} maze_t;

// Déclaration des fonctions concernant le moteur de jeu.
// loadMaze: charge d'un fichier un nouveau plateau de jeu.
maze_t * loadMaze (char * filename);
// unloadMaze: désalloue le plateau de jeu.
void unloadMaze (maze_t * maze);

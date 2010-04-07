// Définition du type tile.
enum tile_e {
	T_EMPTY = 0, 	// Case vide.
	T_HARDWALL, 	// Mur indextructible.
	T_SOFTWALL, 	// Mur destructible.
	T_BOMB,			// Bombe.
	T_BONUS, 		// Bonus.
	T_EXPLOSION,		// Explosion.
	T_PLAYER		// Joueur.
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
  tile_t * t;	// Tile.
} maze_t;

// Nom des joueurs.
const char * namesPlayer[]= {
	"duBoursin",
	"yChieDur",
	"yChieMou",
	"prendDuLaxatif"
};

typedef struct {
	
}

// Déclaration des fonctions concernant le moteur de jeu.
// loadMaze: charge d'un fichier un nouveau plateau de jeu.
maze_t * loadMaze (char * filename);
// unloadMaze: désalloue le plateau de jeu.
void unloadMaze (maze_t * maze);

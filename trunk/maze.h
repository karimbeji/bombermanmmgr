// Définition du type tile.
enum tile_e {
	T_EMPTY = 0, 	// Case vide.
	T_HARDWALL, 	// Mur indextructible.
	T_SOFTWALL, 	// Mur destructible.
	T_BOMB,			// Bombe.
	T_BONUS, 		// Bonus.
	T_EXPLOSION		// Explosion.
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

enum direction_e {
	STOP = 0, 	// Le joueur ne se déplace pas.
	TOP,		// Le joueur va vers le haut.
	BOTTOM,		// Le joueur va vers le bas.
	LEFT,		// Le joueur va vers la gauche.
	RIGHT		// Le joueur va vers la droite.
};

typedef struct {
	int powerBomb; 				// Puissance des bombes.
	enum direction_e direction; // Direction de déplacement.
	int x;						// Position horizontal sur le plateau.
	int y;						// Position vertical sur le plateau.
} player_t;

player_t * arrayPlayer; // Initialisation du tableau de joueur.

// Déclaration des fonctions concernant le moteur de jeu.
// loadMaze: charge d'un fichier un nouveau plateau de jeu.
maze_t * loadMaze (char * filename);
// unloadMaze: désalloue le plateau de jeu.
void unloadMaze (maze_t * maze);
// Vérifie si un joueur n'est pas sur la case suivante.
int checkOtherPlayer(int numPlayer, enum direction_e dir);
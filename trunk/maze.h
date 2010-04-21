// D�finition du type tile.
enum tile_e {
	T_EMPTY = 0, 		// Case vide.
	T_HARDWALL, 		// Mur indextructible.
	T_SOFTWALL, 		// Mur destructible.
	T_BOMB,				// Bombe.
	T_BONUS, 			// Bonus.
	T_EXPLOSION			// Explosion.
};

// D�finit le type d'une case du plateau de jeu.
typedef struct {
	enum tile_e type; 	// Type de tile.
	int power;			// Puissance des bombes.
	int timer;			// Compte � rebour des bombes.
	int bonus;			// D�termine si la case est un bonus.
} tile_t;

// D�finit le plateau de jeu.
typedef struct {
  int w, h;		// Largeur et Hauteur.
  tile_t * t;	// Tile.
} maze_t;

enum direction_e {
	STOP = 0, 	// Le joueur ne se d�place pas.
	UP,		// Le joueur va vers le haut.
	RIGHT,		// Le joueur va vers la droite.
	DOWN,		// Le joueur va vers le bas.
	LEFT,		// Le joueur va vers la gauche.
	MAX_STOP 	// Le joueur ne se d�place pas.
};

typedef struct {
	int alive;					// Indique si le joueur est vivant !
	int powerBomb; 				// Puissance des bombes.
	enum direction_e direction; // Direction de d�placement.
	int x;						// Position horizontal sur le plateau.
	int y;						// Position vertical sur le plateau.
} player_t;

typedef struct {
	int test;	// Test.
	int tile;	// Case.
} explosion_s;

// Initialisation du tableau de joueur.
player_t * arrayPlayer;

// Nombre de joeur;
int nbPlayerDefault;

// V�rifie que 2 joueurs ne se retrouvent pas sur la m�me case.
int checkOtherPlayer(int numPlayer);
// V�rifie que le type de la case soit autoris�.
int checkTileOK (enum tile_e nextCase);
// Explosion d'une bombe.
void explosion (maze_t * maze, int numTile);
// G�n�ration al�atoire des bombes sous les murs destructibles.
int generateBonus (int lucky);
// Calcule la coordonn�e lin�aire � l'aide des coordonn�es X et Y du plateau.
int linearTile (maze_t * maze, int x, int y);
// Charge le plateau de jeu � partir d'un fichier texte.
void loadMaze (char * filename, maze_t * maze);
// D�placement du joueur.
void movePlayer (maze_t * maze, int numPlayer);
// Retourne le type de la prochaine case du joueur.
int nextTileType (maze_t * maze, int numPlayer);
// D�salloue le plateau de jeu.
void unloadMaze (maze_t * maze);
// Met � jour les bombes.
void updateBomb (maze_t * maze);
// Met � jour les les explosions.
void updateExplosion (maze_t * maze);
// Permet d'afficher un peu de texte dans la console.
void updateOutput (int * endGame);
// Met � jour la position du joueur.
void updatePlayer (maze_t * maze, int stepByStep);
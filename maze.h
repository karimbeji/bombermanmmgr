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
  int w,h;		// Largeur et Hauteur.
  tile_t * t;	// Tile.
} maze_t;

enum direction_e {
	STOP = 0, 	// Le joueur ne se d�place pas.
	TOP,		// Le joueur va vers le haut.
	BOTTOM,		// Le joueur va vers le bas.
	LEFT,		// Le joueur va vers la gauche.
	RIGHT		// Le joueur va vers la droite.
};

typedef struct {
	int powerBomb; 				// Puissance des bombes.
	enum direction_e direction; // Direction de d�placement.
	int x;						// Position horizontal sur le plateau.
	int y;						// Position vertical sur le plateau.
} player_t;

player_t * arrayPlayer; // Initialisation du tableau de joueur.

/* Charge un fichier dont on donne le nom et retourne une structure
maze_t correspondant au plateau de jeu. Le fichier doit
correspondre au format donn� par l'�nonc�.
Si le fichier ne peut pas �tre lu, retourne NULL. */
maze_t * loadMaze (char * filename);

// D�salloue le plateau de jeu.
void unloadMaze (maze_t * maze);

// V�rifie si un joueur n'est pas sur la case suivante.
int checkOtherPlayer(int numPlayer, enum direction_e dir);

// V�rifie si le type de la case convient � un d�placement.
int checkTileOK (enum tile_e nextCase);

// G�n�ration al�atoire des bombes sous les murs destructibles...
int generateBonus (int lucky);
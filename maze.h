// Définition du type tile.
enum tile_e {
	T_EMPTY = 0, 		// Case vide.
	T_HARDWALL, 		// Mur indextructible.
	T_SOFTWALL, 		// Mur destructible.
	T_BOMB,				// Bombe.
	T_BONUS, 			// Bonus.
	T_EXPLOSION			// Explosion.
};

// Définit le type d'une case du plateau de jeu.
typedef struct {
	enum tile_e type; 	// Type de tile.
	int power;			// Puissance des bombes.
	int timer;			// Compte à rebour des bombes.
	int bonus;			// Détermine si la case est un bonus.
} tile_t;

// Définit le plateau de jeu.
typedef struct {
  int w, h;		// Largeur et Hauteur.
  tile_t * t;	// Tile.
} maze_t;

enum direction_e {
	STOP = 0, 	// Le joueur ne se déplace pas.
	UP,		// Le joueur va vers le haut.
	RIGHT,		// Le joueur va vers la droite.
	DOWN,		// Le joueur va vers le bas.
	LEFT,		// Le joueur va vers la gauche.
	MAX_STOP 	// Le joueur ne se déplace pas.
};

typedef struct {
	int alive;					// Indique si le joueur est vivant !
	int powerBomb; 				// Puissance des bombes.
	enum direction_e direction; // Direction de déplacement.
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

// Vérifie que 2 joueurs ne se retrouvent pas sur la même case.
int checkOtherPlayer(int numPlayer);
// Vérifie que le type de la case soit autorisé.
int checkTileOK (enum tile_e nextCase);
// Explosion d'une bombe.
void explosion (maze_t * maze, int numTile);
// Génération aléatoire des bombes sous les murs destructibles.
int generateBonus (int lucky);
// Calcule la coordonnée linéaire à l'aide des coordonnées X et Y du plateau.
int linearTile (maze_t * maze, int x, int y);
// Charge le plateau de jeu à partir d'un fichier texte.
void loadMaze (char * filename, maze_t * maze);
// Déplacement du joueur.
void movePlayer (maze_t * maze, int numPlayer);
// Retourne le type de la prochaine case du joueur.
int nextTileType (maze_t * maze, int numPlayer);
// Désalloue le plateau de jeu.
void unloadMaze (maze_t * maze);
// Met à jour les bombes.
void updateBomb (maze_t * maze);
// Met à jour les les explosions.
void updateExplosion (maze_t * maze);
// Permet d'afficher un peu de texte dans la console.
void updateOutput (int * endGame);
// Met à jour la position du joueur.
void updatePlayer (maze_t * maze, int stepByStep);
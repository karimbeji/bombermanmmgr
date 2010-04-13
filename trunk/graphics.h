// Charge les différentes images du jeu...
void loadTiles ();

// Affiche tout le plateau de jeu.
void paint (maze_t * maze);

// Capture les evenements clavier/fenetre.
int getEvent (maze_t * maze);

// Met à jour la position du joueur.
void updatePlayer (maze_t * maze, int stepByStep);

// Met à jour les bombes et les explosions.
void updateBomb (maze_t * maze);

// Explosion d'une bombe.
void explosion (maze_t * maze, int numTile);

// Ouvre une fenêtre avec de taille wxh tuiles de plateau de jeu.
void initWindow (int w, int h);
// Capture les evenements clavier/fenetre.
void getEvent (maze_t * maze, int * finished);
// Ouvre une fen�tre SDL.
void initWindow (int w, int h);
// Charge les d�ff�rentes images du jeu...
void loadTiles ();
// Affiche le plateau du jeu.
void paint (maze_t * maze);
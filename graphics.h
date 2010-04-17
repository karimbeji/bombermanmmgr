// Capture les evenements clavier/fenetre.
void getEvent (maze_t * maze, int * finished);
// Ouvre une fenêtre SDL.
void initWindow (int w, int h);
// Charge les défférentes images du jeu...
void loadTiles ();
// Affiche le plateau du jeu.
void paint (maze_t * maze);
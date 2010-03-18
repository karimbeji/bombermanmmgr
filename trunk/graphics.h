/* Charge les images des tuiles pouvant décrire le plateau de jeu */
void loadTiles();
/* Dessine le plateau de jeu dans la fenetre */
void paint(maze_t *m);
/* Initialise la fenetre de jeu. w et h sont la largeur et la hauteur
   du plateau de jeu, en nombre de tuiles. */
void initWindow(int w,int h);
/* Gere les pressions de touche clavier, et divers evenements */
int getEvent() ;

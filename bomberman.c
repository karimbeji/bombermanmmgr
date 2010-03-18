#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "maze.h"
#include "graphics.h"

int main(int argc, char *argv[]) {
  int finished=0;
  maze_t *maze;
  maze=loadMaze(argv[1]);

  initWindow(maze->w,maze->h);
  loadTiles();
  while (!finished) {
    finished = getEvent();
    paint(maze);
  }
  unloadMaze(maze);
  return 0;
}


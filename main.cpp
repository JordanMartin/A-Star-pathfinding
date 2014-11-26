#include <cstdlib>
#include <cstdio>

#include "maze.hpp"
#include "maze_grid.hpp"
#include "maze_path.hpp"

int main() {

  Maze maze ;
  int width = 20 ;
  int height = 20 ;

  /* initialisation d'un labyrinthe rectangulaire */
  maze_grid_init(maze, height, width) ;
  maze_grid_print(maze, height, width) ;

  /* suppression de murs */
  maze_break_walls_clever(maze) ;
  maze_grid_print(maze, height, width) ;

  /* tests */
  int path_tests = 100 ;
  int path_count = 0 ;

  /* données de chemin */
  PathData path_data ;
  int i ;
  for(i = 0; i < path_tests; ++i) {
    /* points de départ et d'arrivée */
    int start = rand() % maze.tile_size ;
    int end = rand() % maze.tile_size ;
    /* recherche */
    path_count += maze_find_path(maze, start, end, path_data) ;
    /* nettoyage de la recherche */
    maze_path_clean(path_data) ;
  }
  printf("%d chemins trouvés sur %d requêtes\n", path_count, path_tests) ;

  /* recherche de chemin entre deux coins opposés */
  if(maze_find_path(maze, 0, maze.tile_size-1, path_data)) {
    /* chemin trouvé, affichage */
    maze_grid_print_path(maze, height, width, path_data) ;
  } else {
    /* aucun chemin trouvé */
    printf("aucun chemin trouvé entre %d et %d\n", 0, maze.tile_size-1) ;
  }
  maze_path_clean(path_data) ;

  /* nettoyage du labyrinthe */
  maze_grid_clean(maze) ;

  return 0 ;
}

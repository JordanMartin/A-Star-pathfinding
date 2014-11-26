#include <cstdlib>
#include <cstdio>

#include "maze_grid.hpp"
#include "maze_path.hpp"

/* nettoyage des données de recherche de chemin */
void maze_path_clean(PathData & data ) {
  delete data.status;
}

/* affichage d'une case du labyrinthe en fonction des données de chemin */
static void print_path(const void* data,const Tile & tile) {
  /* Récupération du statut du chemin */
  PathData* path_data = (PathData*) data ;
  /* Couleurs des différents statuts */
  const char* status_color[6] = {
    "\x1B[42m", /* vert pour le départ */
    "\x1B[41m", /* rouge pour l'arrivée */
    "\x1B[44m", /* bleu  pour une portion de chemin */
    "\x1B[49m", /* couleur pour une case en dehors */
    "\x1B[49m", /* couleur pour une case inconnue */
    "\x1B[49m", /* couleur pour une case recherchée */
  } ;
  const char* color_clear = "\x1B[49m" ;
  /* mise en place de la couleur selon le statut */
  printf(status_color[path_data->status[tile.index]]) ;
  /* affichage d'un espace */
  printf(" ") ;
  /* remise à zéro de la couleur */
  printf(color_clear) ;
}

/* Affichage d'un chemin */
void maze_grid_print_path(const Maze & maze, int height, int width, const PathData & data ) {
  maze_grid_print_generic(maze, height, width, 1, 2, &data, print_path) ;
}

/* Recherche de chemin */

/* fonction récursive interne */
static int find_path_internal(PathData & data, const Tile & from, const Tile & to ) {
  /* données du chemin pour le point de départ */
  data.status[from.index] = MAZE_PATH_SEARCHED ;
  if(&from == &to) {
    /* nous sommes sur le point d'arrivée */
    /* mise à jour des données du chemin */
    data.status[from.index] = MAZE_PATH_END ;
    return 1 ;
  } else {
    /* nous ne sommes pas sur l'arrivée, recherche à partir des voisins */
    int i ;
    for(i = 0; i < from.neighbor_size; ++i) {
      /* case voisine */
      Tile* neigh = from.neighbors[i] ;
      if(neigh && ! from.walls[i]) {
        /* statut de la case voisine */
        if(data.status[neigh->index] == MAZE_PATH_UNKNOWN) {
          /* ce chemin n'a pas encore été tenté */
          if(find_path_internal(data, *neigh, to)) {
            /* un chemin a été trouvé */
            /* mise à jour des données du chemin */
            data.status[from.index] = MAZE_PATH_IN ;
            return 1 ;
          }
        }
      }
    }
    /* aucun chemin n'a été trouvé */
    data.status[from.index] = MAZE_PATH_OUT ;
    return 0 ;
  }
}

/* fonction de la bibliothèque */
int maze_find_path(const Maze & maze, int start, int end, PathData & data ) {
  int i ;
  /* allocation des données du chemin */
  data.status = new PathStatus[maze.tile_size];
  data.data_size = maze.tile_size ;
  for(i = 0; i < maze.tile_size; ++i) {
    data.status[i] = MAZE_PATH_UNKNOWN ;
  }
  /* départ et arrivée */
  Tile* pfrom = maze.tiles + start ;
  Tile* pto = maze.tiles + end ;
  if(find_path_internal(data, *pfrom, *pto)) {
    /* mise en place du point de départ et d'arrivée */
    data.status[start] = MAZE_PATH_START ;
    data.status[end] = MAZE_PATH_END ;
    return 1 ;
  } else {
    return 0 ;
  }
}


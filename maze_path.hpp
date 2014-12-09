#ifndef _MAZE_PATH_H_
#define _MAZE_PATH_H_

#include "maze.hpp"

/* Données associée à la recherche de chemins */

enum PathStatus { 
  MAZE_PATH_START = 0,   /* la case est le point de départ du chemin */
  MAZE_PATH_END = 1,     /* la case est le point d'arrivée du chemin */
  MAZE_PATH_IN = 2,      /* la case fait partie du chemin */
  MAZE_PATH_OUT = 3,     /* la case ne fait pas partie du chemin */
  MAZE_PATH_UNKNOWN = 4, /* la case n'a pas été visitée */
  MAZE_PATH_SEARCHED = 5, /* la recherche via cette case est en cours */
  MAZE_PATH_EXPLORED = 6
} ;

struct PathData {
  /* statut des cases par rapport au chemin */
  PathStatus* status ;
  /* nombre de cases */
  int data_size ;
} ;

/* Nettoyage des données associées à une recherche de chemin
 */
void maze_path_clean( PathData & data ) ;

/* Recherche d'un chemin entre une case start et une case end.
 *   - la valeur de retour indique si un chemin a été trouvé
 *   - data contient pour chaque case le résultat de la recherche
 */
int maze_find_path(const Maze & maze, int start, int end, PathData & data) ;

/* Affichage d'un chemin dans une grille, d'après les données des cases 
 *   - les données du chemin sont fournies via l'argument data
 */
void maze_grid_print_path(const Maze & maze, int height, int width, const PathData & data) ;

#endif

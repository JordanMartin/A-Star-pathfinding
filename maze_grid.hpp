#ifndef _GRID_MAZE_H_
#define _GRID_MAZE_H_

#include <cstdlib>

#include "maze.hpp"

/* Initialisation d'un labyrinthe rectangulaire à cases carrées :
 *   - height est la hauteur du labyrinthe
 *   - width est la largeur du labyrinthe
 *
 * dans un tel labyrinthe, les cases on en général 4 voisins, 
 */

void maze_grid_init(Maze & maze, int height, int width) ;

/* Libération de la mémoire relative à un labyrinthe.
 * Attention : la libération du champ data des cases n'est pas faite
 */

void maze_grid_clean(Maze & maze) ;

/* Affichage d'un labyrinthe rectangulaire :
 *   - height est la hauteur du labyrinthe
 *   - width est la largeur du labyrinthe
 */

void maze_grid_print(const Maze & maze, int height, int width) ;

/* Affichage générique d'un labyrinthe rectangulaire :
 *   - tile_height est la hauteur des cases
 *   - tile_width est la largeur des cases
 *   - data est un ensemble de données supplémentaires
 *   - print_data est la fonction qui étant donné une case et les
 *     données imprime le caractère de l'intérieur d'une case
 */

void maze_grid_print_generic(const Maze & maze, 
                       int height, 
                       int width, 
                       int tile_height,
                       int tile_width,
                       const void* data,
                       void (*print_data)(const void*,const Tile &)
                     ) ;


#endif

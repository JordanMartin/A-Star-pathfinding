#include <cstdlib>
#include <cstdio>
#include <cstddef>
#include <cstring>
#include <time.h>
#include <iostream>

#include "maze.hpp"
#include "union_find.hpp"

enum POS { TOP = 0, LEFT = 1, BOTTOM = 2, RIGHT = 3 };

/* Destruction des murs */

static void break_wall(Tile & tile, int wall_index) {
  /* détruire le mur pour la case */
  tile.walls[wall_index] = 0 ;
  /* case voisine par ce mur */
  Tile* neigh = tile.neighbors[wall_index] ;
  if(neigh) {
    /* une case se trouve de l'autre côté (pas un mur du bord) */
    /* chercher le mur correspondant pour cette case */
    int w ;
    for(w = 0; neigh->neighbors[w] != &tile; ++w) ;
    neigh->walls[w] = 0 ;
  }
}

/* Destruction naive */

void maze_break_walls_naive(Maze & maze) {
  srand(time(NULL));
  int i ;
  /* pour chaque cellule */
  for(i = 0; i < maze.tile_size; ++i) {
    /* pointeur sur une case */
    Tile* tile = maze.tiles + i ;
    /* tirer un mur au hasard parmis les murs de la case */
    int p = rand()%tile->neighbor_size ;
    /* casser le mur */
    break_wall(*tile, p) ;
  }
}

/* Votre code ici */

/* Destruction intelligente des murs */
void maze_break_walls_clever(Maze & maze) {
	
	srand(time(NULL));
  
	int i, j, index;
	UFNode* neighbor_node = new UFNode[4];
	
	// Initialise les arbres de chaque cases
	UFNode* nodes = new UFNode[maze.tile_size];
	for(i = 0; i < maze.tile_size; ++i) {
		uf_node_init(nodes[i]);
	}
		
	int a = maze.tile_size;
	// pour chaque cellule 
	while(a > 1) {
				
		i = rand() % maze.tile_size;
		
		// pointeur sur une case 
		Tile* tile = maze.tiles + i;
		
		for(j = 0; j < 4; j++){
			if(tile->neighbors[j] != NULL){
				neighbor_node[j] = nodes[tile->neighbors[j]->index];
			}
		}	

		for(int k = 0; k < 4; k++){
			if(tile->neighbors[k] != NULL && 
				!uf_are_equivalent_without_compression(nodes[i], neighbor_node[k])){
					break_wall(*tile, k);
					uf_union(nodes[i], neighbor_node[k]);
					a--;
					std::cout << "Break: " << tile->index << " - " << k << "\n";
			}				
		}
	}  
}

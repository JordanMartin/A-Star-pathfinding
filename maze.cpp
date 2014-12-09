#include <cstdlib>
#include <cstdio>
#include <cstddef>
#include <cstring>
#include <time.h>
#include <iostream>

#include "maze.hpp"
#include "union_find.hpp"

/* Destruction des murs */

void break_wall(Tile & tile, int wall_index) {
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

void restore_wall(Tile & tile, int wall_index) {
  /* détruire le mur pour la case */
  tile.walls[wall_index] = 1 ;
  /* case voisine par ce mur */
  Tile* neigh = tile.neighbors[wall_index] ;
  if(neigh) {
    /* une case se trouve de l'autre côté (pas un mur du bord) */
    /* chercher le mur correspondant pour cette case */
    int w ;
    for(w = 0; neigh->neighbors[w] != &tile; ++w) ;
    neigh->walls[w] = 1 ;
  }
}

void swap_wall(Tile & tile, int wall_index) {
  /* détruire le mur pour la case */
  tile.walls[wall_index] = tile.walls[wall_index] != 0 ? 0 : 1;
  /* case voisine par ce mur */
  Tile* neigh = tile.neighbors[wall_index] ;
  if(neigh) {
    /* une case se trouve de l'autre côté (pas un mur du bord) */
    /* chercher le mur correspondant pour cette case */
    int w ;
    for(w = 0; neigh->neighbors[w] != &tile; ++w) ;
    neigh->walls[w] = neigh->walls[w] != 0 ? 0 : 1;
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
void maze_break_walls_clever(Maze & maze, int supp_wall_count) {
	
	srand(time(NULL));
  
	int i, j, index;
	UFNode* neighbor_node = new UFNode[4];
	
	// Initialise les arbres de chaque case
	UFNode* nodes = new UFNode[maze.tile_size];
	for(i = 0; i < maze.tile_size; ++i) {
		uf_node_init(nodes[i]);
	}
		
	int equiClassCount = maze.tile_size;
	
	int alternativeWayCount = 0;
	
	// Tant qu'il y a plus d'une classe d'équivalence 
	while(equiClassCount > 1 || alternativeWayCount < (supp_wall_count - 1)) {
		
		// Selectionne une case de manière aléatoire
		i = rand() % maze.tile_size;
		
		// Pointeur sur une case 
		Tile* tile = maze.tiles + i;
		
		for(j = 0; j < 4; j++){
			if(tile->neighbors[j] != NULL){
				neighbor_node[j] = nodes[tile->neighbors[j]->index];
			}
		}	

		// On teste tous les murs de la case
		for(int k = 0; k < 4; k++){
			
			// Si le mur est une bordure de la grille on passe
			if(tile->neighbors[k] == NULL){
				continue;
			}
			
			// Si la detruction de ce mur entraine la réunion de deux classes d'équivalence différentes
			if(!uf_are_equivalent_with_compression(nodes[i], neighbor_node[k])){
					
					// Destruction du mur
					break_wall(*tile, k);
					// Les deux classes ne forment maintenant plus qu'une
					uf_union(nodes[i], neighbor_node[k]);
					equiClassCount--;
			}
			// Casse des murs supplémentaires pour créer plusieurs chemins
			else if(equiClassCount == 1){
				break_wall(*tile, k);
				alternativeWayCount++;
			}				
		}
	}  
	
	delete[] neighbor_node;
	delete[] nodes;
}

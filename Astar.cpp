#include "Astar.hpp"


// TODO REMVOE
#include "maze_path.hpp"
#include "maze_grid.hpp"
#include <stdlib.h>
#include <stdio.h>

inline int euclidienne_distance(const Maze& maze, const Tile& t1, const Tile& t2){
	
	int x_start = t1.index % maze.width;
	int y_start = t1.index / maze.width;
	
	int x_end = t2.index % maze.width;
	int y_end = t2.index / maze.width;
	
	int dist = sqrt((x_end - x_start)*(x_end - x_start) + (y_end - y_start)*(y_end - y_start));
	
	return sqrt((t2.altitude - t1.altitude)*(t2.altitude - t1.altitude) + dist*dist);
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



void astar(const Maze& maze, int start_index, int end_index){
	
	ASNODE* nodes = new ASNODE[maze.tile_size];
	
	PathData data;
	// Mémorise le chemin en vue de l'afficher
	data.status = new PathStatus[maze.tile_size];
	data.data_size = maze.tile_size ;

	for(int i = 0; i < maze.tile_size; i++){
		data.status[i] = MAZE_PATH_UNKNOWN;
		nodes[i].color = WHITE;
		nodes[i].parent_index = -1;
		nodes[i].dist = -1;
	}
	
	// TODO TAS BINAIRE
	std::set<int> list_grey;
	//Tile& curr_tile; // = maze.tiles[start_index]
		
	int tmp_dist, tmp_curr_dist_min, curr_index_dist_min;
	
	// Commence avec la première case 
	list_grey.insert(start_index);
	
	// TODO Clever condition d'arrêt
	while(!list_grey.empty()){
		
		// Selection du gris plus proche
		tmp_curr_dist_min = INT_MAX;
		
		for(std::set<int>::iterator it = list_grey.begin(); it != list_grey.end(); ++it){
			
			tmp_dist = euclidienne_distance(maze, maze.tiles[*it], maze.tiles[end_index]);
				
			if(tmp_dist < tmp_curr_dist_min){
				curr_index_dist_min = *it;
				tmp_curr_dist_min = tmp_dist;
			}						
		}		
		
		Tile& curr_tile = maze.tiles[curr_index_dist_min];
		ASNODE& curr_node = nodes[curr_tile.index];
		
		int lsize = list_grey.size();
	
		for(int i = 0; i < curr_tile.neighbor_size; i++){
			
			// Obstacle 
			if(curr_tile.neighbors[i] == NULL){
				continue;
			}
				
			// Noeud noir
			if(nodes[curr_tile.neighbors[i]->index].color == BLACK){
				continue;
			}
				
			nodes[curr_tile.neighbors[i]->index].parent_index = curr_tile.index;
			nodes[curr_tile.neighbors[i]->index].color = GREY;
			
			tmp_dist = nodes[curr_tile.index].dist 
				+ sqrt(1 + pow(curr_tile.neighbors[i]->altitude - curr_tile.altitude, 2));	
			
			if(nodes[curr_tile.neighbors[i]->index].dist == -1 
				|| tmp_dist < nodes[curr_tile.neighbors[i]->index].dist){
					
				// Distance = distance du noeud parent + distance euclidienne entre les deux
				nodes[curr_tile.neighbors[i]->index].dist = tmp_dist;
			}			
			
			if(curr_tile.neighbors[i]->index == end_index){
				// Trouvé !!!
				break;
			}
			
			// TODO WARNING TAS biNAIRE
			// Cette case voisine est accéssible => on la coloris en gris
			list_grey.insert(curr_tile.neighbors[i]->index);
		}
		
		// On a visiter tous ces voisins il devient donc noir
		list_grey.erase(curr_tile.index);
		nodes[curr_tile.index].color = BLACK;
	}
	
	
	
	std::cout << "\n Chemin trouvé : \n";
	int curr_index = end_index;
	
	do{
		data.status[curr_index] = MAZE_PATH_IN;
		std::cout << "[" << curr_index%maze.width << ", " << curr_index/maze.width << "]\n";
		curr_index = nodes[curr_index].parent_index;
	}while(nodes[curr_index].parent_index != -1);
	
	data.status[start_index] = MAZE_PATH_START;
	data.status[end_index] = MAZE_PATH_END;
	
	maze_grid_print_generic(maze, maze.height, maze.width, 1, 2, &data, print_path);
}


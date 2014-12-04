#include "Astar.hpp"


// TODO REMOVE
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

ASNODE* astar(const Maze& maze, int start_index, int end_index, PathData& path_data){
	
	ASNODE* nodes = new ASNODE[maze.tile_size];
	
	// Mémorise le chemin en vue de l'afficher
	path_data.status = new PathStatus[maze.tile_size];
	path_data.data_size = maze.tile_size ;

	for(int i = 0; i < maze.tile_size; i++){
		path_data.status[i] = MAZE_PATH_UNKNOWN;
		nodes[i].color = WHITE;
		nodes[i].parent_index = -1;
		nodes[i].dist = -1;
	}
	
	// TODO TAS BINAIRE
	std::set<int> list_grey;
		
	int tmp_dist, tmp_curr_dist_min, curr_index_dist_min;
	
	// Commence avec la première case 
	list_grey.insert(start_index);
	
	bool way_founded = false;
	
	// TODO Clever condition d'arrêt
	while(!list_grey.empty() && !way_founded){
		
		// Selection du gris le plus proche
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
			
			// Case inaccessible ==> Case hors grille ou mur présent
			if(curr_tile.neighbors[i] == NULL || curr_tile.walls[i] != 0){
				continue;
			}
				
			// Noeud noir
			if(nodes[curr_tile.neighbors[i]->index].color == BLACK){
				continue;
			}
			
			// Distance = distance du noeud parent + distance euclidienne entre les deux
			tmp_dist = nodes[curr_tile.index].dist 
				+ sqrt(1 + pow(curr_tile.neighbors[i]->altitude - curr_tile.altitude, 2));	
			
			// Cette case est déjà grise, est-ce plus cours via la case curr_tile ?
			if(nodes[curr_tile.neighbors[i]->index].color == GREY){
				
				if(nodes[curr_tile.neighbors[i]->index].dist == -1 
				|| tmp_dist < nodes[curr_tile.neighbors[i]->index].dist){
						
					//On met a jour la distance du noeud gris
					nodes[curr_tile.neighbors[i]->index].dist = tmp_dist;
				}	
					
			}else{
				nodes[curr_tile.neighbors[i]->index].parent_index = curr_tile.index;
				nodes[curr_tile.neighbors[i]->index].color = GREY;
			}
								
			// Cette case voisine est la case cherchée
			if(curr_tile.neighbors[i]->index == end_index){
				way_founded = true;
				break;
			}
			
			// TODO WARNING TAS biNAIRE
			// Cette case voisine est accessible => on l'insere dans le tas binaire
			list_grey.insert(curr_tile.neighbors[i]->index);
			path_data.status[curr_tile.neighbors[i]->index] = MAZE_PATH_SEARCHED;
		}
		
		// On a visiter tous ses voisins, il devient donc noir
		list_grey.erase(curr_tile.index);
		nodes[curr_tile.index].color = BLACK;
	}
	
	path_data.status[start_index] = MAZE_PATH_START;
	path_data.status[end_index] = MAZE_PATH_END;

	if(!way_founded){
		return nodes;
	}
	
	// Enregistre le chemin trouvé par remontée depuis le dernier
	int curr_index = nodes[end_index].parent_index;
	
	while(nodes[curr_index].parent_index != -1){
		path_data.status[curr_index] = MAZE_PATH_IN;
		curr_index = nodes[curr_index].parent_index;
		
	};
	
	return nodes;
}


#include "Astar.hpp"

inline int euclidienne_distance(const Maze& maze, const Tile& t1, const Tile& t2){
	
	int x_start = t1.index % maze.width;
	int y_start = t1.index / maze.width;
	
	int x_end = t2.index % maze.width;
	int y_end = t2.index / maze.width;
	
	int dist = sqrt((x_end - x_start)*(x_end - x_start) + (y_end - y_start)*(y_end - y_start));
	
	return sqrt((t2.altitude - t1.altitude)*(t2.altitude - t1.altitude) + dist*dist);
}


void astar(const Maze& maze, int start_index, int end_index){
	
	ASNODE* nodes = new ASNODE[maze.tile_size];
	
	for(int i = 0; i < maze.tile_size; i++){
		nodes[i].color = WHITE;
		nodes[i].parent_index = -1;
		nodes[i].dist = -1;
	}
	
	// TODO TAS BINAIRE
	std::set<int> list_grey;
	Tile& curr_tile = maze.tiles[start_index];
	ASNODE& curr_node = nodes[curr_tile.index];
	
	int tmp_dist, tmp_curr_dist_min, curr_index_dist_min;
	
	do{
	
		for(int i = 0; i < curr_tile.neighbor_size; i++){
			
			// Obstacle 
			if(curr_tile.neighbors[i] != NULL){
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
			list_grey.insert(curr_tile.neighbors[i]->index);
			
			list_grey.erase(curr_tile.index);
		}
		
		nodes[curr_tile.index].color = BLACK;
		
		tmp_curr_dist_min = INT_MAX;
		
		for(std::set<int>::iterator it = list_grey.begin(); it != list_grey.end(); ++it){
			
			tmp_dist = euclidienne_distance(maze, maze.tiles[*it], maze.tiles[end_index]);
				
			if(tmp_dist < tmp_curr_dist_min){
				curr_index_dist_min = *it;
				tmp_curr_dist_min = tmp_dist;
			}						
		}
		
		
	// TODO Clever condition d'arrêt
	}while(!list_grey.empty());
}


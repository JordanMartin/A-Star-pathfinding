#include "astar.hpp"

inline float heurestic(const Maze& maze, const Tile& t1, const Tile& t2){
	
	int x_start = t1.index % maze.width;
	int y_start = t1.index / maze.width;
	
	int x_end = t2.index % maze.width;
	int y_end = t2.index / maze.width;
	 
	float dist = sqrt((x_end - x_start)*(x_end - x_start) + (y_end - y_start)*(y_end - y_start));
	
	return sqrt((t2.altitude - t1.altitude)*(t2.altitude - t1.altitude) + dist*dist);
}

bool compare_heuristic(const std::pair<float,int>& t1, const std::pair<float,int>& t2){
	
	return t1.first > t2.first;
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
		nodes[i].g_cost = 0;
		nodes[i].h_cost = 0;
	}
	
	// File de prio (par tas binaire)
	std::priority_queue<std::pair<int,int>, 
						std::vector<std::pair<int,int> >, 
						decltype(&compare_heuristic)> list_grey(compare_heuristic);
		
	// Commence avec la première case 
	list_grey.push(std::make_pair(0, start_index));
		
	bool way_founded = false;
	int curr_g_cost;
	
	// g(x) distance estimé depuis la case x à la case d'arrivé
	// h(x) distance de la case de départ à la case x
	// f(x) = h(x) + g(x)
	
	while(!list_grey.empty()){
			
		// Selection de la case ayant l'heuristique la plus faible
		Tile& curr_tile = maze.tiles[list_grey.top().second];
		ASNODE& curr_node = nodes[curr_tile.index];
			
		if(curr_tile.index == end_index){
			way_founded = true;
			break;
		}
	
		list_grey.pop();
		curr_node.color = BLACK;
			
		// Parcours les voisins de la case
		for(int i = 0; i < curr_tile.neighbor_size; i++){
			
			// Case hors grille ou derrière un mur
			if(curr_tile.neighbors[i] == NULL || curr_tile.walls[i] != 0){
				continue;
			}
			
			Tile& curr_neighbor_tile = *curr_tile.neighbors[i];
			ASNODE& curr_neighbor_node = nodes[curr_neighbor_tile.index];
			
			// Case déjà visitée
			if(curr_neighbor_node.color == BLACK){
				continue;
			}
								
			curr_g_cost = curr_node.g_cost 
				+ sqrt(1 + pow(curr_neighbor_tile.altitude - curr_tile.altitude, 2));	
			
			// Cette case voisine n'a pas encore été visitée ou offre un cout inférieur 
			if(curr_neighbor_node.color != GREY || 
				curr_g_cost < curr_neighbor_node.g_cost){
			
				// On met a jour les distances g et h
				curr_neighbor_node.g_cost = curr_g_cost;
				curr_neighbor_node.h_cost = heurestic(maze, curr_neighbor_tile, maze.tiles[end_index]);
							
				if(curr_neighbor_node.color != GREY){
					list_grey.push(std::make_pair(
						curr_neighbor_node.g_cost + curr_neighbor_node.h_cost,
						curr_neighbor_tile.index
					));
				}
			
				curr_neighbor_node.parent_index = curr_tile.index;
				curr_neighbor_node.color = GREY;
				path_data.status[curr_neighbor_tile.index] = MAZE_PATH_SEARCHED;
			}
		}
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


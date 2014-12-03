#include "Astar.hpp"


// TODO REMOVE
#include "maze_path.hpp"
#include "maze_grid.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace std;


inline int euclidienne_distance(const Maze& maze, const Tile& t1, const Tile& t2){
	
	int x_start = t1.index % maze.width;
	int y_start = t1.index / maze.width;
	
	int x_end = t2.index % maze.width;
	int y_end = t2.index / maze.width;
	
	int dist = sqrt((x_end - x_start)*(x_end - x_start) + (y_end - y_start)*(y_end - y_start));
	
	return sqrt((t2.altitude - t1.altitude)*(t2.altitude - t1.altitude) + dist*dist);
}

//on supprime l'element de plus grande priorite 
void remove_binary_heap(ASNODE* nodes, vector<index> &binary_heap)
{
	int size=binary_heap.size();
	int i=1;
	int filsd, filsg;
	index element_to_replace;
	int priorite;
	
	binary_heap[1]=binary_heap[size];
	element_to_replace=binary_heap[1];
	 
	binary_heap.resize(binary_heap.size()-1);
    size--;
	
	filsd = 2*i+1;
	filsg = 2*i;
	cout << "size" << size <<endl;
	//s'il n'existe pas de fils gauche, il n'existe pas de fils droit
	while (filsg < size)
	{
		cout << filsg << endl;
		cout << filsd << endl << endl;
		
		if (filsd < size)
		{
			if (nodes[binary_heap[filsd]].dist < nodes[element_to_replace].dist 
			 || nodes[binary_heap[filsg]].dist < nodes[element_to_replace].dist)
			{
				if(nodes[binary_heap[filsg]].dist <= nodes[binary_heap[filsd]].dist)
					{priorite = filsg;}
				else
					{priorite = filsd;}
				
				binary_heap[i] = binary_heap[priorite];
				binary_heap[priorite] = element_to_replace;
				
				i = priorite;
				element_to_replace = binary_heap[i];
				
				filsg = 2*i;
				filsd=2*i+1;
			}
			
			else 
			{break;}
	   }
	   else
	   {
		   if (nodes[binary_heap[filsg]].dist < nodes[element_to_replace].dist)
			{					
				binary_heap[i] = binary_heap[filsg];
				binary_heap[filsg] = element_to_replace;
				break;
		    }
		   else 
			{break;}
		}
	}
}

void insert_binary_heap(ASNODE* nodes, vector<index> &binary_heap, index element)
{
	int i, ancester;
	binary_heap.resize(binary_heap.size()+1);
	
	i = binary_heap.size();
	
	ancester = i/2; 
	while ((ancester >= 1) && (nodes[binary_heap[ancester]].dist > nodes[element].dist))
	{
		binary_heap[i]=binary_heap[ancester];
		i=ancester;
		ancester = i/2;
	}
	binary_heap[i]=element;
}

ASNODE* astar(const Maze& maze, index start_index, index end_index, PathData& path_data){
	
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
	
	//TAS BINAIRE, premiere case vide pour des questions de praticité
	vector<index> list_grey;
	list_grey.push_back(-1);
	//Tile& curr_tile; // = maze.tiles[start_index]
		
	int tmp_dist, tmp_curr_dist_min, curr_index_dist_min;
	
	// Commence avec la première case 
	list_grey.push_back(start_index);
	
	bool way_founded = false;
	
	// TODO Clever condition d'arrêt
	while(list_grey.size()!=1 && !way_founded){
		
		Tile& curr_tile = maze.tiles[list_grey[1]];
		ASNODE& curr_node = nodes[curr_tile.index];
	
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
				insert_binary_heap(nodes, list_grey,curr_tile.neighbors[i]->index);
			}
								
			// Cette case voisine est la case cherchée
			if(curr_tile.neighbors[i]->index == end_index){
				way_founded = true;
				break;
			}

			// Cette case voisine est accessible => on l'insere dans le tas binaire

			path_data.status[curr_tile.neighbors[i]->index] = MAZE_PATH_SEARCHED;
		}
		
		// On a visiter tous ses voisins, il devient donc noir 
		// Et on supprime l'element de plus grande priorite
		remove_binary_heap(nodes, list_grey);
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


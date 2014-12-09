#ifndef _ASTAR_H_
#define _ASTAR_H_

#include <cstdlib>
#include <cmath>
#include <set>
#include <vector>
#include <iostream>
#include <queue>

#include "maze.hpp"
#include "maze_path.hpp"

typedef int index;

enum ASColor { BLACK, GREY, WHITE };
//TODO FIX NAME
struct ASNODE {
	
	int parent_index; // -1 = pas de parent
	int g_cost; // Meilleure distance depuis le noeud de départ  (-1 = infini)
	float h_cost; // Heuristique distance
	ASColor color;
};

ASNODE* astar(const Maze& maze, int start_index, int end_index, PathData& path_data, float& search_time);


#endif

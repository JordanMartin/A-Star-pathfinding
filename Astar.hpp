#ifndef _ASTAR_H_
#define _ASTAR_H_

#include <cstdlib>
#include <cmath>
#include "maze.hpp"
#include <set>
#include <climits>

enum ASColor { BLACK, GREY, WHITE };
//TODO FIX NAME
struct ASNODE {
	
	int parent_index; // -1 = pas de parent
	int dist; // Meilleure distance depuis le noeud de départ  (-1 = infini)
	ASColor color;
};

struct Astar {
	
	ASNODE *start, *end;
};

void astar(const Maze& maze, int start_index, int end_index);


#endif

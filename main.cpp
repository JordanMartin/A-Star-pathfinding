#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

#include "maze.hpp"
#include "maze_grid.hpp"
#include "maze_path.hpp"
#include "Astar.hpp"

using namespace std;

void display_menu();

int ask_for_menu_item();
void do_action(Maze& maze, int action_id);
void set_grid_size(Maze& maze);
void ask_coordinate(string msg, int* coord);
void press_key_to_continue();

void new_empty_grid(Maze& maze);
void new_random_grid(Maze& maze);
void edit_grid(Maze& maze);
void display_grid(Maze& maze);
void start_astar(Maze& maze);

int main() {

	Maze maze;
	maze.tiles = NULL;
	
	int action_id;
	
	do{
		display_menu();	
		action_id = ask_for_menu_item();
		do_action(maze, action_id);
		press_key_to_continue();		
		
	}while(action_id != 9);
	
	return 0 ;
}

void display_menu(){
	
	system("clear");	
	cout << "============================================================" << endl; 
	cout << "              Menu principale : Algorithme A*               " << endl; 
	cout << "============================================================" << endl << endl; 
	
	cout << "  1: Créer une nouvelle grille vide" << endl;
	cout << "  2: Générer une nouvelle grille aléatoirement" << endl;
	cout << "  3. Modifier la grille (murs et altitudes des cases)" << endl << endl;
	cout << "  4. Afficher la grille" << endl;
	
	cout << "  5. Lancer l'algorithme Astar entre deux cases" << endl << endl;
	
	cout << "  9. Quitter" << endl << endl;
	
	cout << "------------------------------------------------------------" << endl; 
}

int ask_for_menu_item(){
	
	int choice;
	cout << "Votre choix : ";
	cin >> choice;
	
	return choice; 
}

void do_action(Maze& maze, int action_id){
	
	switch(action_id){
		case 3:
		case 4:
		case 5:
			if(maze.tiles == NULL){
				cout << "/!\\ Veuillez créer ou générer une grille avant d'effectuer cette action" << endl;
				return;
			}
		break;
		default:
		break;
	}
	
	
	switch(action_id){
		
		case 1: new_empty_grid(maze); break;
		case 2: new_random_grid(maze); break;
		case 3: edit_grid(maze); break;		
		
		case 4: display_grid(maze); break;	
		case 5: start_astar(maze); break;
	
		case 9: 
			if(maze.tiles !=  NULL) maze_grid_clean(maze); 
		break;
		
		default:
		break;		
	}
}

void set_grid_size(Maze& maze){
	
	cout << "Largeur de la grille : ";
	cin >> maze.width;
	
	cout << "Hauteur de la grille : ";
	cin >> maze.height;
}

void ask_coordinate(string msg, int* coord){
	int readed;
	do{
		cout << msg << " : x,y : ";
		cin.ignore();
		readed = scanf("%d,%d", &coord[0], &coord[1]);		
	}while(readed != 2);
}

inline int coord_to_index(const Maze& maze, int x, int y){
	return x + maze.width*y;
}


void new_empty_grid(Maze& maze){
	int nb_walls;
	set_grid_size(maze);
	maze_grid_init(maze, maze.height, maze.width);
	 /* suppression des murs */
    for (int i=0; i<maze.tile_size; i++)
    {
		for(int j=0; j<4; ++j) 
		{
			if (maze.tiles[i].neighbors[j] != NULL)
			maze.tiles[i].walls[j] = 0;
		}
    }
	display_grid(maze);
}

void new_random_grid(Maze& maze){
	set_grid_size(maze);
	maze_grid_init(maze, maze.height, maze.width);
	
	int way_count;
	cout << "Nombre de murs supplementaires à casser : ";
	cin >> way_count;
	
	maze_break_walls_clever(maze, way_count);
	display_grid(maze);
}

void edit_grid(Maze& maze){

	int coord[2];
	
	display_grid(maze);
	
	ask_coordinate("Coordonées de la case à éditer", coord);
	int tile_index = coord_to_index(maze, coord[0], coord[1]);
	
	int opt_id;
	
	do{
		system("clear");
		cout << "========================================================" << endl; 
		cout << "    Modification de la case (" << coord[0] << "," << coord[1] << ") - Altitude = " << maze.tiles[tile_index].altitude << endl; 
		cout << "========================================================" << endl << endl; 
		
		cout << " 1. Casser tous les murs de cette case" << endl;
		cout << " 2. Restaurer tous les murs de cette case" << endl << endl;
		
		if(maze.tiles[tile_index].walls[0] != 0)
			cout << " 3. Casser le mur haut" << endl;
		else
			cout << " 3. Restaurer le mur haut" << endl;
		
		if(maze.tiles[tile_index].walls[1] != 0)	
			cout << " 4. Casser le mur gauche" << endl;
		else
			cout << " 4. Restaurer le mur gauche" << endl;
		
		if(maze.tiles[tile_index].walls[2] != 0)
			cout << " 5. Casser le mur bas" << endl;
		else
			cout << " 5. Restaurer le mur bas" << endl;
		
		if(maze.tiles[tile_index].walls[3] != 0)
			cout << " 6. Casser le mur droit" << endl << endl;
		else
			cout << " 6. Restaurer le mur droit" << endl << endl;
							
		cout << " 7. Modifier l'altitude" << endl << endl;
		
		cout << " 8. Modifier une autre case" << endl << endl;
			
		cout << " 9. Valider" << endl;
		
		display_grid(maze);
		
		opt_id = ask_for_menu_item();
		
		switch(opt_id){
			case 1:
				break_wall(maze.tiles[tile_index], 0);
				break_wall(maze.tiles[tile_index], 1);
				break_wall(maze.tiles[tile_index], 2);
				break_wall(maze.tiles[tile_index], 3);
			break;
			
			case 2:
				restore_wall(maze.tiles[tile_index], 0);
				restore_wall(maze.tiles[tile_index], 1);
				restore_wall(maze.tiles[tile_index], 2);
				restore_wall(maze.tiles[tile_index], 3);
			break;
						
			case 3: swap_wall(maze.tiles[tile_index], 0); break;
			case 4: swap_wall(maze.tiles[tile_index], 1); break;
			case 5: swap_wall(maze.tiles[tile_index], 2); break;
			case 6: swap_wall(maze.tiles[tile_index], 3); break;
			
			case 7: 
				cout << "Nouvelle altitude : ";
				cin >> maze.tiles[tile_index].altitude;
			break;
			
			case 8:
				ask_coordinate("Coordonées de la nouvelle case à éditer", coord);
				tile_index = coord_to_index(maze, coord[0], coord[1]);
			break;
			
			default:
			break;
	}
		
	}while(opt_id != 9);
}

void display_grid(Maze& maze){
	cout << endl;
	maze_grid_print(maze, maze.height, maze.width);
	cout << endl;
}

void start_astar(Maze& maze){
	
	PathData path_data;

	display_grid(maze);
	
	int start_index, end_index;
	int start_coord[2], end_coord[2];
	
	int readed;
		
	ask_coordinate("Coordonnées de départ", start_coord);
	ask_coordinate("Coordonnées d'arrivée", end_coord);

	start_index = coord_to_index(maze, start_coord[0], start_coord[1]);
	end_index = coord_to_index(maze, end_coord[0], end_coord[1]);
	
	ASNODE* nodes = astar(maze, start_index, end_index, path_data);
	
	cout << endl;
	
	if(nodes[end_index].parent_index != -1){
		cout << "Chemin trouvé !" << endl;
	}else{
		cout << "Chemin non trouvé !" << endl;
	}
	
	cout << endl;
	
	maze_grid_print_path(maze, maze.height, maze.width, path_data);
	delete[] nodes;
	maze_path_clean(path_data);
}

void press_key_to_continue(){
	cout << endl << endl << "[Appuyez sur une touche pour continuer]";
	cin.ignore();
	getchar();
}

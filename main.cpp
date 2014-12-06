#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

#include "maze.hpp"
#include "maze_grid.hpp"
#include "maze_path.hpp"
#include "astar.hpp"

using namespace std;

void display_menu();

int ask_for_menu_item();
void do_action(Maze& maze, int action_id);
void set_grid_size(Maze& maze);
void ask_coordinate(string msg, int* coord);
void ask_number(string msg, int& n);
void press_key_to_continue();

void new_empty_grid(Maze& maze);
void new_full_wall_grid(Maze& maze);
void new_random_grid(Maze& maze);
void edit_grid(Maze& maze);
void display_grid(Maze& maze);
void start_astar(Maze& maze);

int main() {

	Maze maze;
	maze.tiles = NULL;
	
	/*
	maze.height = 15;
	maze.width = 60;
	//TODO TMP REMOVE
	maze_grid_init(maze, maze.height, maze.width, true	);
	// suppression des murs
    for (int i=0; i < maze.tile_size; i++)
    {
		for(int j=0; j < 4; ++j) 
		{
			if (maze.tiles[i].neighbors[j] != NULL)
			maze.tiles[i].walls[j] = 0;
		}
    }
    
    // Trace un U en obstacle
    restore_wall(maze.tiles[5*60+30], 0);restore_wall(maze.tiles[5*60+30], 1);restore_wall(maze.tiles[5*60+30], 2);restore_wall(maze.tiles[5*60+30], 3);
    restore_wall(maze.tiles[6*60+30], 0);restore_wall(maze.tiles[6*60+30], 1);restore_wall(maze.tiles[6*60+30], 2);restore_wall(maze.tiles[6*60+30], 3);
    restore_wall(maze.tiles[7*60+30], 0);restore_wall(maze.tiles[7*60+30], 1);restore_wall(maze.tiles[7*60+30], 2);restore_wall(maze.tiles[7*60+30], 3);
    restore_wall(maze.tiles[8*60+30], 0);restore_wall(maze.tiles[8*60+30], 1);restore_wall(maze.tiles[8*60+30], 2);restore_wall(maze.tiles[8*60+30], 3);
    restore_wall(maze.tiles[9*60+30], 0);restore_wall(maze.tiles[9*60+30], 1);restore_wall(maze.tiles[9*60+30], 2);restore_wall(maze.tiles[9*60+30], 3);
    
    restore_wall(maze.tiles[5*60+29], 0);restore_wall(maze.tiles[5*60+29], 1);restore_wall(maze.tiles[5*60+29], 2);restore_wall(maze.tiles[5*60+29], 3);
    restore_wall(maze.tiles[5*60+28], 0);restore_wall(maze.tiles[5*60+28], 1);restore_wall(maze.tiles[5*60+28], 2);restore_wall(maze.tiles[5*60+28], 3);
    restore_wall(maze.tiles[5*60+27], 0);restore_wall(maze.tiles[5*60+27], 1);restore_wall(maze.tiles[5*60+27], 2);restore_wall(maze.tiles[5*60+27], 3);
    restore_wall(maze.tiles[5*60+26], 0);restore_wall(maze.tiles[5*60+26], 1);restore_wall(maze.tiles[5*60+26], 2);restore_wall(maze.tiles[5*60+26], 3);
    
    restore_wall(maze.tiles[9*60+29], 0);restore_wall(maze.tiles[9*60+29], 1);restore_wall(maze.tiles[9*60+29], 2);restore_wall(maze.tiles[9*60+29], 3);
    restore_wall(maze.tiles[9*60+28], 0);restore_wall(maze.tiles[9*60+28], 1);restore_wall(maze.tiles[9*60+28], 2);restore_wall(maze.tiles[9*60+28], 3);
    restore_wall(maze.tiles[9*60+27], 0);restore_wall(maze.tiles[9*60+27], 1);restore_wall(maze.tiles[9*60+27], 2);restore_wall(maze.tiles[9*60+27], 3);
    restore_wall(maze.tiles[9*60+26], 0);restore_wall(maze.tiles[9*60+26], 1);restore_wall(maze.tiles[9*60+26], 2);restore_wall(maze.tiles[9*60+26], 3);	
    
    
    
	//~ int supp_wall_count= 1500;
	//~ maze_break_walls_clever(maze, supp_wall_count);
	//~ 
	//TODO
	*/
	
	int action_id;
	
	do{
		display_menu();	
		action_id = ask_for_menu_item();
		do_action(maze, action_id);
		
		if(action_id != 9){
			press_key_to_continue();		
		}	
		
	}while(action_id != 9);
	
	return 0 ;
}

void display_menu(){
	
	system("clear");	
	cout << "============================================================" << endl; 
	cout << "              Menu principale : Algorithme A*               " << endl; 
	cout << "============================================================" << endl << endl; 
	
	cout << "  1: Créer une nouvelle grille sans murs" << endl;
	cout << "  2: Créer une nouvelle grille avec tous les murs" << endl;
	cout << "  3: Générer une nouvelle grille aléatoirement" << endl;
	cout << "  4. Modifier la grille (murs et altitudes des cases)" << endl << endl;
	cout << "  5. Afficher la grille" << endl;
	
	cout << "  6. Lancer l'algorithme Astar entre deux cases" << endl << endl;
	
	cout << "  9. Quitter" << endl << endl;
	
	cout << "------------------------------------------------------------" << endl; 
}

int ask_for_menu_item(){
	
	int choice;
	ask_number("Votre choix", choice);
		
	return choice; 
}

void do_action(Maze& maze, int action_id){
	
	switch(action_id){
		case 4:
		case 5:
		case 6:
			if(maze.tiles == NULL){
				cout << "/!\\ Veuillez créer ou générer une grille avant d'effectuer cette action" << endl;
				return;
			}
		break;	
	}
	
	switch(action_id){
		
		case 1: new_empty_grid(maze); break;
		case 2: new_full_wall_grid(maze); break;
		case 3: new_random_grid(maze); break;
		case 4: edit_grid(maze); break;		
		
		case 5: display_grid(maze); break;	
		case 6: start_astar(maze); break;
	
		case 9: 
			if(maze.tiles !=  NULL) maze_grid_clean(maze); 
		break;	
	}
}

void set_grid_size(Maze& maze){
	
	ask_number("Largeur de la grille", maze.width);
	ask_number("Hauteur de la grille", maze.height);
}

void ask_coordinate(string msg, int* coord){
	int readed;
	do{
		cout << msg << " : x,y : ";
		cin.ignore();
		readed = scanf("%d,%d", &coord[0], &coord[1]);		
	}while(readed != 2);
}

void ask_number(string msg, int& n){
	
	while(1){
		cout << msg << " : ";
		cin >> n;

		if(!cin.fail())		
			return;
		
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Nombre invalide...\n";
	}
}

inline int coord_to_index(const Maze& maze, int x, int y){
	return x + maze.width*y;
}


void new_empty_grid(Maze& maze){
	
	int nb_walls;
	set_grid_size(maze);
	maze_grid_init(maze, maze.height, maze.width);
	
	// suppression des murs
    for (int i=0; i < maze.tile_size; i++)
    {
		for(int j=0; j < 4; ++j) 
		{
			if (maze.tiles[i].neighbors[j] != NULL)
			maze.tiles[i].walls[j] = 0;
		}
    }
    
	display_grid(maze);
}

void new_full_wall_grid(Maze& maze){
	
	set_grid_size(maze);
	maze_grid_init(maze, maze.height, maze.width);    
	display_grid(maze);
}

void new_random_grid(Maze& maze){
	
	set_grid_size(maze);
	
	int variable_altitude;
	ask_number("Affecter une altitude aléatoire pour chaque case ? (1: oui, 0; non)", variable_altitude);
	
	maze_grid_init(maze, maze.height, maze.width, (bool)variable_altitude);
	
	int supp_wall_count;
	ask_number("Nombre de murs supplementaires à casser", supp_wall_count);
	
	maze_break_walls_clever(maze, supp_wall_count);
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
		
		cout << "  1. Casser tous les murs de cette case" << endl;
		cout << "  2. Restaurer tous les murs de cette case" << endl << endl;
		
		if(maze.tiles[tile_index].walls[0] != 0)
			cout << "  3. Casser le mur haut" << endl;
		else
			cout << "  3. Restaurer le mur haut" << endl;
		
		if(maze.tiles[tile_index].walls[1] != 0)	
			cout << "  4. Casser le mur gauche" << endl;
		else
			cout << "  4. Restaurer le mur gauche" << endl;
		
		if(maze.tiles[tile_index].walls[2] != 0)
			cout << "  5. Casser le mur bas" << endl;
		else
			cout << "  5. Restaurer le mur bas" << endl;
		
		if(maze.tiles[tile_index].walls[3] != 0)
			cout << "  6. Casser le mur droit" << endl << endl;
		else
			cout << "  6. Restaurer le mur droit" << endl << endl;
							
		cout << "  7. Modifier l'altitude" << endl << endl;
		
		cout << "  8. Modifier une autre case" << endl;			
		cout << "  9. Valider" << endl << endl;
		
		cout << "------------------------------------------------------------" << endl;
		
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
				ask_number("Nouvelle altitude", maze.tiles[tile_index].altitude);
			break;
			
			case 8:
				ask_coordinate("Coordonées de la nouvelle case à éditer", coord);
				tile_index = coord_to_index(maze, coord[0], coord[1]);
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
	cin.ignore(INT_MAX, '\n');
	getchar();
}

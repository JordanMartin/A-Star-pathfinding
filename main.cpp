#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <climits>

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
void display_grid(const Maze& maze, int highlight_start_index = -1, int highlight_end_index = -1);
void start_astar(Maze& maze);

/**
 * Selectionne uen case avec les flèches 
 * 
 * @param start_index Si supérieur à -1 affiche en vert cette case 
 * @param end_index Si supérieur à -1 affiche en bleu cette case 
 * 
 **/
int interactive_select_tile(Maze& maze, string msg, int start_index, int end_index = -1);

enum KeyPress {UP, DOWN, LEFT, RIGHT, ENTER, UNDEFINED};
/**
 * Detecte l'appuie sur les touches du clavier
 * Si aucun paramètre n'est pas passé, la fonction retourne la flèche appuyée ou la touche entrée
 * 
 * @param immediat_return Si true, la fonction retourne dès qu'une flêche ou la touche entrée est appuyée. 
 *  		Si false, elle retourne seulement lorsque la touche entrée est appuyée
 * 
 * @param {on_up, on_right, on_down, on_left} Callback appelé lorsque la touche correspondante est appuyée
 **/
KeyPress listen_direction_key(void* args = NULL, 
							bool immediat_return = true,
							void on_up(void* args) = NULL, 
							void on_right(void* args) = NULL,
							void on_down(void* args) = NULL,
							void on_left(void* args) = NULL);

static inline int mod(int a, int b){
	return a >= 0 ? a % b : b - (-a % b);
}

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
	
	/*
	ask_coordinate("Coordonées de la case à éditer", coord);
	int tile_index = coord_to_index(maze, coord[0], coord[1]);
	*/
	int tile_index = interactive_select_tile(maze, "Selectionnez la case à editer", -1);
	coord[0] = tile_index % maze.width;
	coord[1] = tile_index / maze.width;
	
	
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
		
		display_grid(maze, tile_index);
		
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
			/*
				ask_coordinate("Coordonées de la nouvelle case à éditer", coord);
				tile_index = coord_to_index(maze, coord[0], coord[1]);
				* */
				tile_index = interactive_select_tile(maze, "Selectionnez la case à editer", -1);
				coord[0] = tile_index % maze.width;
				coord[1] = tile_index / maze.width;
			break;
	}
		
	}while(opt_id != 9);
}

void display_grid(const Maze& maze, int highlight_start_index, int highlight_end_index){
	
	cout << endl;
		
	if(highlight_start_index != -1 || highlight_end_index != -1){			
		PathData path_data;
		path_data.status = new PathStatus[maze.tile_size];
		path_data.data_size = maze.tile_size;
		
		for(int i = 0; i < maze.tile_size; i++){
			path_data.status[i] = MAZE_PATH_UNKNOWN;
		}
		
		if(highlight_start_index != -1){
			path_data.status[highlight_start_index] = MAZE_PATH_START;
		}
		
		if(highlight_end_index != -1){
			path_data.status[highlight_end_index] = MAZE_PATH_END;
		}
		
		maze_grid_print_path(maze, maze.height, maze.width, path_data);
		maze_path_clean(path_data);
	}else{
		maze_grid_print(maze, maze.height, maze.width);
	}
}

void start_astar(Maze& maze){
	
	PathData path_data;
	
	int start_index = 0, end_index = 0;
	int start_coord[2], end_coord[2];
	int readed;
		
	start_index = interactive_select_tile(maze, "Selectionnez la case de départ", -1, -1);
	cout << start_index << endl;
	end_index = interactive_select_tile(maze, "Selectionnez la case de d'arriver", start_index, -1);
	cout << end_index << endl;	
	
	/* OLD INTPUT MODE	
	ask_coordinate("Coordonnées de départ", start_coord);
	ask_coordinate("Coordonnées d'arrivée", end_coord);
	start_index = coord_to_index(maze, start_coord[0], start_coord[1]);
	end_index = coord_to_index(maze, end_coord[0], end_coord[1]);
	*/
	
	float search_time;
	ASNODE* nodes = astar(maze, start_index, end_index, path_data, search_time);
	
	cout << endl;
	
	if(nodes[end_index].parent_index != -1){
		printf("Chemin trouvé en %.2fms\n", search_time);	
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

KeyPress listen_direction_key(void* args, 
							bool immediat_return,
							void on_up(void* args), 
							void on_right(void* args),
							void on_down(void* args),
							void on_left(void* args)){
	
	system("stty -icanon");
	system("stty -echo");

	cin.ignore();
	char c;
	char hist[5];
	int hist_pos = 0;
	int choice;
	
	KeyPress last_pressed = UNDEFINED;
		
	do{ 
		c = getchar();
		hist[hist_pos] = c;
		
		if(c == 10){
			last_pressed = ENTER;
		}
		
		if(hist[mod(hist_pos-1, 4)] == 91){
			
			switch(hist[hist_pos]){
				case 65:
					if(on_up != NULL) on_up(args);
					last_pressed = UP;
				break;
				
				case 66:
					if(on_down != NULL) on_down(args);
					last_pressed = DOWN;
				break;
				
				case 67: 
					if(on_right != NULL) on_right(args);
					last_pressed = last_pressed = RIGHT;
				break;
				
				case 68: 
					if(on_left != NULL) on_left(args);
					last_pressed = last_pressed = LEFT;
				break;
			}
		}
			
		++hist_pos %= 4;
		
	}while(last_pressed == UNDEFINED || (!immediat_return && last_pressed != ENTER));
	
	system("stty icanon");
	system("stty echo");
	
	return last_pressed;						  
}

int interactive_select_tile(Maze& maze, string msg, int start_index, int end_index){
	
	int init_index = 0;
	
	KeyPress key;
	
	do{
		system("clear");
		if(start_index == -1) display_grid(maze, init_index);
		else display_grid(maze, start_index, init_index);
		
		cout << msg << " (Utilisez les flêches, 2x Entrée pour valider)\n";
		key = listen_direction_key();
		
		switch(key){
			case UP:
				init_index -= maze.width;
			break;
			
			case DOWN:
				init_index += maze.width;
			break;
			
			case LEFT:
				init_index--;
			break;
			
			case RIGHT:
				init_index++;
			break;
		}
		
		init_index = mod(init_index, maze.tile_size);
		
	}while(key != ENTER);
	
	return init_index;
}

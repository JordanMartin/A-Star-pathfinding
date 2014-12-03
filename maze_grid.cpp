#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "maze_grid.hpp"

#define MAX_ALTITUDE 21

/* case d'un labyrinthe en forme de grille, de position i,j */
static Tile* grid_maze_tile(const Maze & maze, int height, int width, int i, int j) {
  if(i >= 0 && i < width && j >= 0 && j < height) {
    /* dedans */
    return maze.tiles + j*width + i ;
  } else {
    /* dehors */
    return NULL ;
  }
}

/* initialization d'un labyrinthe rectangulaire */
void maze_grid_init(Maze & maze, int height, int width) {
	
  srand(time(NULL));

  /* retour rapide dans le cas d'un labyrinthe vide */
  if(width == 0 || height == 0) return ;

  /* allocation des cases */
  maze.tiles = new Tile[height*width];
  maze.tile_size = height*width ;

  /* allocation de tous les voisinages d'un coup */
  Tile** maze_neighbors = new Tile*[4*width*height];

  /* allocation de tous les murs d'un coup */
  int* maze_walls = new int[4*width*height];

  /* allocation et mise en place des voisinages */
  int i, j, k ;
  for(j = 0; j < height; ++j) {
    for( i = 0; i < width; ++i) {
      /* indice de la case */
      int index = j*width + i ;
      /* pointeur sur la case */
      Tile* tile = maze.tiles + index ;
      tile->index = index ;
      
      // Altitude (aléatoire) de la case
      tile->altitude = rand() % MAX_ALTITUDE;

      /* allocation des voisinages et des murs */
      tile->neighbors = maze_neighbors + 4*index ;
      tile->walls = maze_walls + 4*index ;
      tile->neighbor_size = 4 ;

      /* initialisation des voisinages */
      /* les voisins sont rangés dans l'ordre haut, gauche, bas, droite */
      tile->neighbors[0] = grid_maze_tile(maze, height, width, i  , j-1) ;
      tile->neighbors[1] = grid_maze_tile(maze, height, width, i-1, j  ) ;
      tile->neighbors[2] = grid_maze_tile(maze, height, width, i  , j+1) ;
      tile->neighbors[3] = grid_maze_tile(maze, height, width, i+1, j  ) ;

      /* initialisation des murs */
      for(k = 0; k < 4; ++k) {
        tile->walls[k] = 1 ;
      }
    }
  }
}

/* Libération de la mémoire d'unlabyrinthe rectangulaire */
void maze_grid_clean(Maze & maze) {
  /* déallocation de la mémoire des cases */
  delete[] maze.tiles[0].neighbors;
  delete[] maze.tiles[0].walls;
  delete[] maze.tiles;
}

/* Affichage générique d'un labyrinthe */
void maze_grid_print_generic(const Maze & maze, 
                       int height, 
                       int width, 
                       int tile_height,
                       int tile_width,
                       const void* data,
                       void (*print_data)(const void*,const Tile&)
                     ) {

  /* caractères pour l'affichage des murs */
#ifdef MAZE_SIMPLE_DISPLAY
  const char* wall_chars[18] = {
    " ", "-", " ", "+", "-", "-", "+", "+", " ",
    "+", "|", "+", "+", "+", "+", "+", "-" ,"|"
  } ;
#else
  const char* wall_chars[18] = {
    " ", 
    "\xe2\x95\xb6", "\xe2\x95\xb7", "\xe2\x95\xad", 
    "\xe2\x95\xb4", "\xe2\x94\x80", "\xe2\x95\xae", 
    "\xe2\x94\xac", "\xe2\x95\xb5", "\xe2\x95\xb0", 
    "\xe2\x94\x82", "\xe2\x94\x9c", "\xe2\x95\xaf", 
    "\xe2\x94\xb4", "\xe2\x94\xa4", "\xe2\x94\xbc", 
    "\xe2\x94\x80" ,"\xe2\x94\x82"
  } ;
#endif

  /* une case fait deux caractères de large */
  int i, j, k, l ;
  
  printf("    ");
  // Affichage des numéros de colonnes
  for(i = 0; i < width; ++i) {
    if(i/10 != 0)
	  printf("  %d", i/10);
	else
	  printf("   ");
  }
  printf("\n");
  printf("    ");
  // Affichage des numéros de colonnes
  for(i = 0; i < width; ++i) {
	  printf("  %d", i%10);
  }
  printf("\n");
  
  /*itération sur les lignes*/
  for(j = 0; j <= height; ++j) {
	  
	printf("    ");
	  
    /* affichage des murs du haut */
    /* pointeurs sur les cases autour du coin haut gauche*/
    Tile *br = NULL, *bl = NULL, *tr = NULL, *tl = NULL ;
    for(i = 0; i <= width; ++i) {
      tl = tr ;
      bl = br ;
      tr = grid_maze_tile(maze, height, width, i, j-1) ;
      br = grid_maze_tile(maze, height, width, i, j  ) ;

      /* types des murs autour du coin, encodé sur quatre bits */
      int corner_type = 0 ;
      if((tr && tr->walls[1]) || (tl && tl->walls[3])) ++corner_type ;
      corner_type *= 2 ;
      if((tl && tl->walls[2]) || (bl && bl->walls[0])) ++corner_type ;
      corner_type *= 2 ;
      if((bl && bl->walls[3]) || (br && br->walls[1])) ++corner_type ;
      corner_type *= 2 ;
      if((br && br->walls[0]) || (tr && tr->walls[2])) ++corner_type ;

      /* affichage du coin */
      printf(wall_chars[corner_type]) ;

      /* mur du haut */
      if(i < width) {
        if((tr && tr->walls[2]) || (br && br->walls[0])) {
          for(k = 0; k < tile_width; ++k) {
            printf(wall_chars[16]) ;
          }
        } else {
          for(k = 0; k < tile_width; ++k) {
            printf(" ") ;
          }
        }
      }
    }
    
    if( j != height)    printf("\n") ;

	// Affichage du numéro de ligne
	if(j < height) printf("%3d ", j);
		
    if(j < height) {
      /* affichage des murs droite gauche */
      for(k = 0; k < tile_height; ++k) {
        Tile *prev = NULL, *current = NULL ;
        for(i = 0; i <= width; ++i) {
          prev = current ;
          current = grid_maze_tile(maze, height, width, i, j) ;
          if((prev && prev->walls[3]) || (current && current->walls[1])) {
            printf(wall_chars[17]) ;
          } else {
            printf(" ") ;
          }
          if(i < width) {
            for(l = 0; l < tile_width; ++l) {
              /* affichage de l'intérieur de la case en utilisant print_data */
              print_data(data, *current) ;
            }
          }
        }
        // Affichage du numéro de ligne
		printf("%d ", j);
        printf("\n") ;
      }
    }
  }
  printf("\n");
   printf("    ");
  // Affichage des numéros de colonnes
  for(i = 0; i < width; ++i) {
    if(i/10 != 0)
	  printf("  %d", i/10);
	else
	  printf("   ");
  }
  printf("\n");
  printf("    ");
  // Affichage des numéros de colonnes
  for(i = 0; i < width; ++i) {
	  printf("  %d", i%10);
  }
  printf("\n");
}

/* fonction par défaut d'affichage des cases */
static void print_none(const void* data, const Tile & tile) {
  printf(" ") ;
}

/* affichage basique d'un labyrinthe rectangulaire */
void maze_grid_print(const Maze & maze, int height, int width ) {
  maze_grid_print_generic(maze, height, width, 1, 2, NULL, print_none) ;
}


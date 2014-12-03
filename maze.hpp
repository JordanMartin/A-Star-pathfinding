#ifndef _MAZE_H_
#define _MAZE_H_

/* ===== Labyrinthes =====
 *
 * Votre but dans ce TP est de générer automatiquement des labyrinthes
 * "intéressants". Par intéressant, nous voulons dire que si vous prenez deux
 * cases quelconques du labyrinthe, il soit toujours possible de trouver un
 * chemin reliant l'une à l'autre. De plus, nous voulons que ce chemin soit
 * unique, et donc qu'étant donné une entrée et une sortie, il n'y ait qu'une
 * seule solution pour résoudre le labyrinthe et donc trouver le chemin.
 *
 * ===  Structures de données ===
 *
 * Un labyrinthe est constitué d'un ensemble de cases. Chaque case possède un
 * certain nombre de cases voisines, ainsi qu'un indicateur, mentionnant si un
 * mur se trouve entre les deux cases, ou s'il est possible de passer de l'une à
 * l'autre. Chaque case est également dotée d'un indice entier, unique pour
 * chaque case, qui vous permettra de stocker des données dans des tableaux
 * auxiliaires pour associer des données aux cases.
 *
 * === Méthode ===
 *
 * Vous commencerez avec un labyrinthe où toutes les cases sont séparées par des
 * murs. Vous ferez ensuite tomber des murs, un par un. Vous vous assurerez à
 * chaque fois que vous cassez un mur que les deux cases que ce mur sépare ne
 * sont pas au préalable reliées par un autre chemin. En examinant tous les murs
 * du labyrinthe dans un ordre aléatoire, vous pourrez alors garantir que le
 * labyrinthe n'a pas une structure triviale, et que vous aurez cassé
 * suffisamment de murs pour que les cases soient toutes mutuellement
 * accessibles.
 *
 * === Outils ===
 *
 * La partie algorithmique de ce TP consiste à implémenter l'algorithme d'union
 * find. Une description de cet algorithme et de ses objectifs est founie dans
 * le fichier union_find.h, et vous pourrez trouver plus de détails si vous en
 * voulez :
 *   - sur Wikipedia pour une description plus détaillée
 *   - dans le livre "Algorithmique" de Cormen, Leiserson, Rivest et Stein,
 *     chapitre 21 : structures de données pour ensembles disjoints
 *
 * Cet algorithme et les structures de données associées vous permettront
 * rapidement de répondre à la question :
 *
 * "étant donné deux cases, et les murs cassés jusqu'à présent, ces cases
 * sont-elles reliées par un chemin dans le labyrinthe ?"
 *
 * === Matériel fourni et contraintes ===
 *
 * L'ensemble des headers du projet sont fournis, et n'ont pas besoin d'être
 * modifiés. 
 *
 * Les fichiers maze_grid.[hc] vous fournissent les outils nécessaires
 * pour initialiser un labyrinthe en forme de grille rectangulaire (cases
 * carrées), pour libérer la mémoire associée, ainsi que pour afficher de tels
 * labyrinthes. 
 *
 * Les fichiers maze_path.[hc] contiennent du code pour rechercher
 * automatiquement un chemin entre deux cases du labyrinthe, ainsi que pour
 * afficher un tel chemin. Vous n'avez normalement pas à vous soucier du contenu
 * de ces fichiers, ou à assimiler le code qu'ils contiennent. 
 *
 * Le fichier main.c utilise l'ensemble des fichiers fournis pour tester la
 * création d'un labyrinthe, et tester l'existence de chemins entre les cases du
 * labyrinthe. Il vous donne également des exemples d'utilisation des fonctions
 * et procédures fournies dans. L'image expected_output.png vous donne une idée
 * du résultat auquel vous attendre une fois que l'ensemble sera en place.
 *
 */

/* Le type Tile décrit une case :
 *   - neighbors fournit les pointeurs sur les cases voisines dans le labyrinthe
 *     un pointeur nul signifie qu'il n'y a pas de voisin
 *   - walls indique si un mur se situe entre la case et le voisin correspondant
 *   - neighbor_size fournit la taille du tableau de voisins
 *   - index est l'indice de la case parmis l'ensemble des cases, et vous permet
 *     d'indexer un tableau contenant des données associées aux cases
 */

struct Tile {
  Tile** neighbors ;
  int* walls ;
  int neighbor_size ;
  int index ;
  int altitude;
} ;

/* Le type Maze décrit un labyrinthe :
 *   - tiles est un tableau contenant toutes les cases
 *   - tile_size indique le nombre de cases du labyrinthe
 */

struct Maze {
  Tile* tiles ;
  int tile_size ;
  int width, height;
} ;

/* Destruction des murs du labyrinthe 
 * Attention, on ne suppose pas nécessairement ici que le labyrinthe est basé
 * sur une grille de cases.
 */

/* Procédure naive :
 * pour chaque tuile, un mur est détruit au hasard
 */

void maze_break_walls_naive(Maze & maze) ;

/* Procédure intelligente :
 * à la fin de cette procédure, pour toute paire de cases, il existe un et
 * unique chemin permettant de se rendre de l'une à l'autre. Pour réaliser cette
 * opération, vous utiliserez l'algorithme d'union find :
 *
 * - la relation d'équivalence est : "est relié par un chemin à"
 * - initialisation : 
 *   - tous les murs sont présents
 *   - chaque case est dans sa propre classe d'équivalence
 * - commencer par faire la liste de tous les murs du labyrinthe
 * - mélanger cet ensemble de murs
 *   - si les cases de part et d'autre sont reliées par un chemin, le garder
 *   - sinon casser le mur, et fusionner les classes des deux cases
 *
 * De cette manière, on s'assure :
 * - que chaque mur cassé n'ouvre pas de nouveau chemin entre deux cases déjà
 *   reliées ;
 * - que deux cases sont toujours reliées, car on traite tous les murs
 */

void maze_break_walls_clever(Maze & maze, int alternative_way_count) ;

void break_wall(Tile & tile, int wall_index);
void restore_wall(Tile & tile, int wall_index);
void swap_wall(Tile & tile, int wall_index);

#endif

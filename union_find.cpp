#include <cstdlib>
#include <iostream>

#include "union_find.hpp"

/* initialization */

void uf_node_init( UFNode & node ) {
  	node.parent = &node;
	node.height = 1;
}

/* union
 * après l'appel, node1 et node2 sont dans la même classe 
 */

void uf_union( UFNode & node1, UFNode & node2 ) {
 
	UFNode *root, *sub_root;
	
	if(node1.height > node2.height){
		root = &uf_get_root_with_compression(node1);
		sub_root = &uf_get_root_with_compression(node2);
	}else{
		root = &uf_get_root_with_compression(node2);
		sub_root = &uf_get_root_with_compression(node1);
	}	
	
	root->height += sub_root->height;
	sub_root->parent = root;
	
}

/* find */

/* parcourir l'arbre de la classe et renvoyer la racine */
UFNode & uf_get_root(UFNode & node ) {
  
	UFNode* root = &node;
	UFNode* curr_node = &node;
	
	while(curr_node != curr_node->parent){
		curr_node = curr_node->parent;
		root = curr_node;
	}
	
	return *root ;
}

/* parcourir l'arbre de la classe et renvoyer la racine */
const UFNode & uf_get_root_const(const UFNode & node ) {
  
	const UFNode* root = &node;
	const UFNode* curr_node = &node;
	
	while(curr_node != curr_node->parent){
		curr_node = curr_node->parent;
		root = curr_node;
	}
	
	return *root ;
}

/* parcourir l'arbre de la classe en faisant au fur et a mesure la compression et renvoyer la racine */
UFNode & uf_get_root_with_compression(UFNode & node ) {
	
	
	if (&node == node.parent)	{return node;}
	else
	{
		node.parent = &uf_get_root_with_compression(*node.parent);
		return *node.parent;
	}
}

/* vérifier si deux noeuds sont dans la même classe */
int uf_are_equivalent_without_compression(const UFNode & node1,const UFNode & node2 ) {
	return &uf_get_root_const(node1) == &uf_get_root_const(node2) ;
}

int uf_are_equivalent_with_compression(UFNode & node1,UFNode & node2 ) {
  return &uf_get_root_with_compression(node1) == &uf_get_root_with_compression(node2) ;
}

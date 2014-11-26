#ifndef _UNION_FIND_H_
#define _UNION_FIND_H_

/* ===== Algorithme d'union find et relations d'équivalence =====
 *
 * Le but de l'algorithme union-find est la gestion de classes d'équivalence. 
 *
 * === Classes d'équivalence ===
 *
 * Étant donné un ensemble d'éléments (de type quelconque), une relation
 * d'équivalence sur ces éléments respecte les critères suivants :
 *
 * - symétrie     : si x est en relation avec y, y est en relation avec x ;
 *
 * - transitivité : si x est en relation avec y et y est en relation avec z
 *                  alors x est en relation avec z ;
 *
 * - réflexivité  : x est en relation avec x.
 *
 * Par exemple, sur un ensemble d'étudiants, la relation "est dans le même
 * groupe de TP que" est une relation d'équivalence : 
 * - si Bilal est dans le même groupe de TP que Maelle, Maelle est dans le 
 *   même groupe de TP que Bilal ;
 * - si Maelle est également dans le même groupe de TP que Ming, Bilal est 
 *   dans le même groupe de TP que Ming ;
 * - Ming est dans le même groupe de TP que Ming.
 *
 * Une relation d'équivalence définit un ensemble de classes d'équivalences. Ces
 * classes sont des sous ensemble des éléments, tels que deux éléments de la
 * même classe sont équivalents, et deux éléments de classes différentes ne sont
 * pas équivalents. Dans l'exemple précédent des groupes de TP, les classes
 * d'équivalence sont les groupes de TP eux-mêmes. Les classes d'équivalence
 * définissent une partition de l'ensemble des éléments : chaque élément
 * appartient à une classe d'équivalence, un élément ne peut pas appartenir à
 * plusieurs classes d'équivalences.
 *
 * === Objectif de l'algorithme d'union find ===
 *
 * Le but de l'algorithme d'union find est de construire, à partir d'un ensemble
 * d'éléments et d'un ensemble de paires d'éléments équivalents, l'ensemble des
 * classes d'équivalence. Il doit donc répondre à deux requêtes :
 *
 * - find  : étant donné deux éléments, déterminer si ces deux éléments sont
 *           dans la même classe d'équivalence. 
 *
 * - union : étant donné une paire d'éléments équivalents, fusionner les classes
 *           d'équivalence de ces deux éléments ;
 *
 * Il s'agit donc de faire en sorte que les complexités de ces opérations soient
 * les plus faibles possibles.
 *
 * === principe et mise en oeuvre ===
 *
 * Dans l'algorithme, les classes d'équivalence sont encodées sous forme
 * d'arbres. Contrairement aux arbres classiques que vous commencez à avoir
 * l'habitude de manipuler, ces arbres sont encodés de bas en haut : chaque
 * noeud de l'arbre possède l'information pour atteindre son parent. La racine
 * de l'arbre est son propre parent. Ces arbres ne sont donc pas nécessairement 
 * des arbres binaires, car plus de noeuds peuvent avoir le même parent. Muni 
 * de ces arbres, les deux opérations sont réalisées de la manière suivante :
 *
 * - find  : étant donné deux éléments, pour chaque élément, remonter les liens 
 *           de parenté jusqu'à la racine de l'arbre. Si les racines des deux
 *           arbres sont les mêmes, les éléments sont équivalents.
 *
 * - union : étant donné deux éléments, comme pour find, les racines de leurs
 *           arbres respectifs sont recherchées. L'union est réalisée en
 *           affectant le parent de l'une des racines comme l'autre racine.
 *
 * === Complexité et optimisation ===
 *
 * Comme pour la plupart des algorithmes sur les arbres, la complexité des
 * opérations réalisées dépend de la profondeur de l'arbre. Ici, les deux
 * opérations (union et find) nécessitent de parcourir les arbres de bas en
 * haut, jusqu'à la racine. Dans le pire des cas, la recherche est lancée à
 * partir d'une feuille de l'arbre, et toute la hauteur de l'arbre est
 * parcourue. Il est donc important de faire en sorte que la hauteur des arbres
 * soit la plus faible possible. Deux optimisations sont donc proposées.
 *
 * = Compression de chemins =
 *
 * L'algorithme classique pour la recherche de la racine de l'arbre d'un élément
 * est le suivant :
 *
 * Fonction racine_classe( noeud : un élément )
 * Début
 * | si noeud.parent == noeud :
 * | | retourner noeud
 * | sinon :
 * | | retourner racine_classe(noeud.parent)
 * | fin si
 * Fin
 *
 * La compression de chemin modifie cet algorithme pour faire en sorte que suite
 * à cette recherche, tous les noeuds rencontrés sur le chemin de la racine
 * prennent la racine pour parent, et se retrouvent donc à profondeur 1. Le
 * nouvel algorithme est donc le suivant :
 *
 * Fonction racine_classe( noeud : un élément ) : noeud, le représentant
 * Début
 * | si noeud.parent == noeud :
 * | | retourner noeud
 * | sinon :
 * | | noeud.parent <- racine_classe(noeud.parent)
 * | | retourner noeud.parent
 * | fin si
 * Fin
 *
 * = Union contrôlée =
 *
 * La seconde optimisation consiste à maintenir une approximation de la
 * profondeur des arbres. Lors de l'union, la racine de l'arbre le moins profond
 * devient la fille de l'arbre de la racine de l'arbre le plus profond, ce qui
 * n'augmente pas la profondeur. Si les deux arbres sont de même profondeur,
 * l'approximation de la profondeur de la racine qui devient la parente est
 * augmentée de 1. Le pseudo-code de l'union avec cette optimisation devient
 * alors :
 *
 * Procedure fusion( noeud1, noeud2 : deux éléments )
 * Début
 * | racine1 <- racine_classe(noeud1)
 * | racine2 <- racine_classe(noeud2)
 * | si(racine1 != racine2)
 * | | si racine1.profondeur < racine2.profondeur :
 * | | | racine1.parent <- racine2
 * | | sinon si racine2.profondeur < racine1.profondeur :
 * | | | racine2.parent <- racine1
 * | | sinon
 * | | | racine2.parent <- racine1
 * | | | racine1.profondeur <- racine1.profondeur + 1
 * | | fin si
 * | fin si
 * Fin
 *
 * = Complexité finale =
 *
 * Muni de ces deux optimisations, la complexité amortie des opérations d'union
 * et de recherche devient O(a(n)), où n est le nombre d'éléments et a(n) est la
 * réciproque de la fonction d'Ackermann. En pratique, pour n valant le nombre
 * d'atomes de l'univers, a(n) vaut 5, et cette complexité est donc généralement
 * considérée comme du temps constant.
 *     
 * */

/* Un élément :
 * - parent est un pointeur vers le noeud parent dans l'arbre de la classe
 * - rank est une approximation de l'arbre enraciné au noeud
 * - data est un champ géénrique à utiliser à la convenance de l'utilisateur
 * */

struct UFNode {
  UFNode* parent ;
  int height ;
} ;

/* initialization */

void uf_node_init( UFNode & node ) ;

/* union 
 * après l'appel, node1 et node2 sont dans la même classe 
 */

void uf_union(UFNode & node1,UFNode & node2 ) ;
/* find */

/* parcourir l'arbre de la classe et renvoyer la racine */
UFNode & uf_get_root(UFNode & node ) ;
const UFNode & uf_get_root_const(const UFNode & node ) ;

/* vérifier si deux noeuds sont dans la même classe */
int uf_are_equivalent_without_compression(const UFNode & node1,const UFNode & node2 ) ;

int uf_are_equivalent_with_compression(UFNode & node1,UFNode & node2 ) ;

#endif

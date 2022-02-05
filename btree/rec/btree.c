/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) { // > right, < left, == out #DONE
  if (tree == NULL) return false;

  
  if (key < tree->key) { // left
    return bst_search(tree->left, key, value);
  }
  else if (key > tree->key) { // right
    return bst_search(tree->right, key, value);
  }
  else if (key == tree->key) { // out 
    *value = tree->value;
    return true;
  }
  
  return false; // just to avoid warn
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) { // #DONE
  if (tree == NULL) return;

  if (*tree == NULL) { // creating
    bst_node_t *element = malloc(sizeof(struct bst_node));
    if(!element) return;

    element->left = NULL;
    element->right = NULL;
    element->key = key;
    element->value = value;
    *tree = element;
    return;
  }
  else if ((*tree)->key > key) bst_insert(&((*tree)->left), key, value); // go left
  else if ((*tree)->key < key) bst_insert(&((*tree)->right), key, value); // go right
  else if ((*tree)->key == key) (*tree)->value = value; // overwrite
  
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) { // #DONE
  if (tree == NULL) return;

  if ((*tree)->right == NULL) {
    target->key = (*tree)->key;
    target->value = (*tree)->value;
    bst_node_t *element = *tree;
    (*tree)=(*tree)->left;
    free(element);
  }
  else if ((*tree)->right != NULL) bst_replace_by_rightmost(target, &(*tree)->right);

}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) { // L R inverted #DONE
  if (tree == NULL || (*tree) == NULL) return; // to avoid segfault

  if (key < (*tree)->key) bst_delete(&((*tree)->left), key); // go right
  if (key > (*tree)->key) bst_delete(&((*tree)->right), key); // go left
  if (key == (*tree)->key) { // parse for leaves the node we need to delete
    
    if ((*tree)->left == NULL && (*tree)->right == NULL) { // if we dont have podstromy
      free(*tree); 
      *tree = NULL;
      return;
    }

    if ((*tree)->right == NULL) { // if right podstrom is empty
      bst_node_t *element = (*tree)->left; // temp
      free(*tree);
      *tree = element;
      return;
      //*tree = element; //going left from temp/tree
      //free(element); //deleting temp
    }

    if ((*tree)->left == NULL) { // if left podstrom is empty
      bst_node_t *element = (*tree)->right; // temp
      free(*tree);
      *tree = element;
      return;
      //*tree = element->right; //going right from temp/tree
      //free(*tree); //deleting temp
    }

    if ((*tree)->left != NULL && (*tree)->right != NULL) {
      bst_replace_by_rightmost((*tree), &((*tree)->left));
      return;
    }
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  if (tree == NULL) return;

  free(*tree);
	*tree = NULL;

  return;
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  if (tree == NULL) return;


}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  if (tree == NULL) return;


}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  if (tree == NULL) return;

  
}

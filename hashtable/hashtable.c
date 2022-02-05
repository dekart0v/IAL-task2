/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
  if (table == NULL) return;
  else {
    for (int i = 0; i < HT_SIZE; i++) {
      (*table)[i]  = NULL;
    }
  }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  if (table == NULL) return NULL;

  ht_item_t *result = (*table)[get_hash(key)]; // variable for search results
  while (result != NULL) { // if not the end of the table
    if (strcmp(result->key, key) == 0) return result; // 0 means equal; if element key = search key -> return this element
    result = result->next; // if found nothing jump to the next element
  }
  return 0; // just to not have a warning
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */ 
void ht_insert(ht_table_t *table, char *key, float value) {
  if (table == NULL) return;

  ht_item_t *element = ht_search(table, key); // get element (will be empty if we dont have one)

  if (element != NULL) element->value = value; // overwrite existing element
  else if (element == NULL) {
    ht_item_t *element = (ht_item_t*)malloc(sizeof(ht_item_t)); // allocate memory for new element
    element->key = key; // assign values
    element->value = value;
    element->next = (*table)[get_hash(key)]; // t.array[t.n] ← el
    (*table)[get_hash(key)] = element; // t.n ← t.n+1
  }

}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  if (table == NULL) return NULL;

  ht_item_t *element = ht_search(table, key);
  if (!element) return NULL;
  else return &element->value;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  if (table == NULL) return;

  ht_item_t *result = (*table)[get_hash(key)];
  ht_item_t *element = NULL; // temp element (previous)

  if (result == NULL) return; // halt if we dont have this element

  while (strcmp(result->key,key)) { // while != reassign values 
    element = result; // prev = current
    result = result->next; // current = next
  } // parse till ==

  element->next = result->next; // t.n ← t.n-1

  free(result);
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
  if (table == NULL) return;

  ht_item_t *result = NULL;
  ht_item_t *element = NULL;

  for (int i = 0; i < HT_SIZE; i++) {
    result = (*table)[i];
    while (result != NULL) { // while we still have smth
        element = result->next; // next element
        free(result);
        result = element; //after deliting result switch to new result which was stored at <<element>>
    }
    (*table)[i] = NULL;
  }
}

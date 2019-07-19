#ifndef AUX_POLYGON_H
#define AUX_POLYGON_H


#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "../include/ei_types.h"
#include "hw_interface.h"



/* structure de cotes */
typedef struct ei_cote_t
{
  int y_max;
  int y_min;
  int x_y_min;
  int delta_x;
  int delta_y;
  int x_intersect;
  int erreur;
} ei_cote_t;



/*
permet de gerer les cotés, listes de cotés et le dessin de polygones
*/

/* suite chainée de cotes */
typedef struct ei_linked_cote_t
{
  ei_cote_t cote;
  struct ei_linked_cote_t * next;
} ei_linked_cote_t;



/* calcul de la taille de la liste de cotes*/
int len_liste_cotes(ei_linked_cote_t *liste);


/* free de la memeoire d'une liste chainée de cotés */
void free_liste_cotes(ei_linked_cote_t *liste_cote);


/* actualise les abscisses des intersection de la liste de cotes avec la scanline */
void calcul_intersections(ei_linked_cote_t *cote, int scanline);


/* ajoute en tete de TCA les cotes de la liste cotes_rajouter */
void rajouter_cotes_TCA(ei_linked_cote_t **p_TCA, ei_linked_cote_t *cotes_rajouter);


/* supprime de TCA les cotés dont le y_max est égal à la scanline */
void supprimer_cotes_TCA(ei_linked_cote_t **p_TCA, int scanline);


/* trie par TCA par ordre croissant des abscisses d'intersection avec la scanline */
void tri_TCA(ei_linked_cote_t **TCA);


/* renvoie le coté max de TCA (par ordre d'intersection avec la scanline) */
ei_linked_cote_t* cote_max_TCA(ei_linked_cote_t* TCA);


/* supprime un coté unique de TCA */
void supprime_cote_solo_TCA(ei_linked_cote_t **TCA, ei_linked_cote_t *cote_supprimer);


/* colorie une scanline */
void colorier_scanline_TCA(ei_surface_t surface, ei_linked_cote_t *TCA, int scanline, ei_color_t color, const ei_rect_t *clipper);


#endif

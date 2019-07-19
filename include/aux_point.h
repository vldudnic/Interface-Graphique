#ifndef AUX_POINT_H
#define AUX_POINT_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "ei_types.h"
#include "hw_interface.h"


/*
permet de gerer les points et les listes de points
*/



/* modifie la valeur RGBA d'un pixel de la surface aux coordonnées de point et de la couleur color */
void change_pixel_value(ei_surface_t surface, const ei_point_t point, const ei_color_t color);


/* Verifie si un point est dans le clipper */
bool is_in_clipper(ei_point_t, const ei_rect_t *clipper);



/* calcul de la taille de la liste de points*/
int len_liste_points(ei_linked_point_t *liste);


/* renvoie une liste contenant les éléments contenus entre début compris, fin compris */
ei_linked_point_t* prendre_intervale_liste_points(ei_linked_point_t *liste, int debut, int fin);


/* ajoute un point solo en tete de la liste */
void ajouter_point_solo_tete(ei_linked_point_t **p_liste, ei_point_t point);


/* on insère en queue de p_liste (non vide) la liste liste_ajoute */
void inserer_liste_points_queue(ei_linked_point_t **p_liste, ei_linked_point_t *liste_ajoute);


/* free de la memeoire d'une liste chainée de points */
void free_liste_points(ei_linked_point_t *liste_points);

#endif

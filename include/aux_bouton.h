#ifndef AUX_BOUTON_H
#define AUX_BOUTON_H

#include <stdlib.h>

#include "ei_types.h"
#include "hw_interface.h"

/*
fonctions permettant de gerer le dessin des boutons et plus généralement le dessin de rectangles avec:
                                                                                    - bordures,
                                                                                    - effet relief
                                                                                    - angles arrondis

+ la fonction where_to_place qui n'est pas vraiment "à sa place"
                                                                         
*/


/* genere une liste de points representant un arc de cercle de rayon rayon entre les angles debut et fin */
ei_linked_point_t* ei_draw_arc(ei_point_t origine, int rayon, double debut, double fin);


/* genere une liste de points represantant un rectangle
        le paramettre rayon permet d'arrondir les angles en entrant un rayon > 0
*/
ei_linked_point_t*  rectangle_arrondi(ei_rect_t rectangle, int rayon);


/* dessine le rectangle rectangle avec une bordure de taille border_width de couleur color
        on peut décider d'un effet de relief avec le parametre relief
        le paramettre rayon permet d'arrondir les angles en entrant un rayon > 0
*/
void ei_draw_button_shape(ei_surface_t surface, const ei_rect_t rectangle, const int rayon, const int border_width, const ei_color_t color, ei_relief_t relief, ei_rect_t* clipper);


/* renvoie le point où placer un objet de taille object_size dans un rectangle destination en fonction de l'ancre anchor */
ei_point_t where_to_place(ei_size_t object_size, ei_rect_t destination, ei_anchor_t anchor);

#endif

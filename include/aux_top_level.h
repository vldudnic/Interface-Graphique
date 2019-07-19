#ifndef AUX_TOP_LEVEL_H
#define AUX_TOP_LEVEL_H

#include "ei_types.h"
#include "ei_widget.h"
#include "ei_event.h"
#include "ei_top_level.h"


/*
quelques fonction auxiliaires permettant de savoir dans quelle partie d'une top level se trouve la souris
*/

// définit les boutons repère de notre top_level
void definition_points_top_level(ei_toplevel_widget_t *top_level);

// nous renvoie si la souris est dans le petit carré de redimension
ei_bool_t souris_dans_carre_redim(ei_toplevel_widget_t *top_level, ei_mouse_event_t position_souris);

// nous renvoie si la souris est dans la bordure du haut de la top_level
ei_bool_t souris_dans_bordure(ei_toplevel_widget_t *top_level, ei_mouse_event_t position_souris);

// nous renvoie si la souris est dans le bouton rouge avec la croix
ei_bool_t souris_dans_bouton_croix(ei_toplevel_widget_t *top_level, ei_mouse_event_t position_souris);


#endif

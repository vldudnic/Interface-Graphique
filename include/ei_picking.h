#ifndef EI_PICKING_H
#define EI_PICKING_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../include/ei_types.h"
#include "../include/ei_widget.h"
#include "../include/ei_application.h"
#include "../include/hw_interface.h"
#include "../include/ei_draw.h"
#include "../include/ei_event.h"
#include "../include/ei_bouton.h"


/* permet de gerer le picking de widgets*/


//liste chainée de widget
typedef struct ei_linked_widget_t {
  ei_widget_t* widget;
  struct ei_linked_widget_t* next;
} ei_linked_widget_t;


/* test d'égalité entre deux couleur */
bool are_colors_equal(ei_color_t color1, ei_color_t color2);


/* handle l'event */
void ei_handle_event(ei_event_t *event);

/* la souris est elle dans le bouton? */
ei_bool_t souris_dans_bouton(ei_bouton_widget_t *bouton, ei_mouse_event_t position_souris);

/* dessine tous les widgets de la liste_widget dans la surface et la pick_surface */
void ei_draw_all_widgets(ei_surface_t surface, ei_surface_t pick_surface, ei_linked_widget_t *liste_widgets, ei_rect_t *clipper);

/* renvoie true si les noms des classes name1 et name2 sont les mêmes */
bool are_widgetclass_names_equal(ei_widgetclass_name_t name1, ei_widgetclass_name_t name2);

/* place le widget widget en tete de la liste des widgets à afficher */
void ei_event_set_front_widget(ei_widget_t *widget);

#endif

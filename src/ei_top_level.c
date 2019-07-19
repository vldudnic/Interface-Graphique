#include "../include/hw_interface.h"
#include "../include/ei_draw.h"
#include "../include/ei_types.h"
#include "../include/ei_widgetclass.h"
#include "../include/ei_widget.h"
#include "../include/ei_top_level.h"
#include "../include/aux_bouton.h"
#include "../include/aux_top_level.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

extern ei_widgetclass_t* widgetclass_list;
extern int pick_id;
extern ei_color_t pick_color;
extern ei_bool_t draw_event;
ei_point_t coin_dim_avant;

ei_widgetclass_t ei_widgetclass_toplevel = {"top_level", &top_level_alloc, &top_level_release, &top_level_draw, &top_level_setdefault, &top_level_geomnotify, &top_level_handle};


void* top_level_alloc()
{
  return calloc(1, sizeof(ei_toplevel_widget_t));
}


void top_level_release(struct ei_widget_t* widget)
{
  ei_toplevel_widget_t *toplevel = widget;
  ei_widget_t *first_child = toplevel->widget.children_head;
  while (first_child != NULL)
  {
    struct ei_widget_t *temp = first_child;
    first_child = first_child->next_sibling;
    temp->wclass->releasefunc(temp);
  }
  free(toplevel->min_size);
}





void top_level_draw(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper)
{
  ei_toplevel_widget_t *top_level = widget;

  int bordure_haut = 22 + 2*top_level->border_width;
  ei_rect_t grand_rect;
  grand_rect.top_left.x = top_level->widget.screen_location.top_left.x - top_level->border_width;
  grand_rect.top_left.y = top_level->widget.screen_location.top_left.y - bordure_haut;

  grand_rect.size.width = top_level->widget.screen_location.size.width + 2*top_level->border_width;
  grand_rect.size.height = top_level->widget.screen_location.size.height + bordure_haut + top_level->border_width;

  ei_color_t grey_color = {100, 100, 100, 255};
  ei_color_t red_color = {255, 0, 0, 255};

  ei_draw_button_shape(surface, grand_rect, 0, 0, grey_color, ei_relief_none, clipper);


  if (top_level->closable)
  {
    ei_point_t centre_bouton;
    ei_rect_t bouton;
    int rayon = 8;

    centre_bouton.x = top_level->widget.screen_location.top_left.x + rayon + 2;
    centre_bouton.y = top_level->widget.screen_location.top_left.y - rayon - 2 - top_level->border_width;
    bouton.top_left = centre_bouton;
    bouton.size.width = 0;
    bouton.size.height = 0;

    ei_draw_button_shape(surface, bouton, rayon, 2, red_color, ei_relief_raised, clipper);
  }

  ei_draw_button_shape(surface, top_level->widget.screen_location, 0, 0, top_level->color, ei_relief_none, clipper);


  int largeur_carre;

  if (top_level->widget.requested_size.height < top_level->widget.requested_size.width)
  {
    largeur_carre = (top_level->widget.requested_size.height)/12;
  }
  else
  {
    largeur_carre = (top_level->widget.requested_size.width)/12;
  }

  ei_rect_t carre_redimensionnement;
  carre_redimensionnement.size.width = largeur_carre;
  carre_redimensionnement.size.height = largeur_carre;
  carre_redimensionnement.top_left.x = grand_rect.top_left.x + grand_rect.size.width - largeur_carre;
  carre_redimensionnement.top_left.y = grand_rect.top_left.y + grand_rect.size.height - largeur_carre;

  ei_draw_button_shape(surface, carre_redimensionnement, 0, 0, grey_color, ei_relief_none, clipper);

  ei_draw_button_shape(pick_surface, grand_rect, 0, 0, *(top_level->widget.pick_color), ei_relief_none, clipper);


}


void top_level_setdefault(struct ei_widget_t* widget)
{
  ei_toplevel_widget_t *top_level = widget;

  top_level->widget.wclass = &ei_widgetclass_toplevel;
  top_level->widget.pick_id = pick_id;
  pick_id ++;
  ei_color_t *new_pick_color = malloc(sizeof(ei_color_t));
  *new_pick_color = pick_color;
  top_level->widget.pick_color = new_pick_color;

  if (pick_color.red < 255)
  {
    pick_color.red ++;
  }
  else
  {
    if (pick_color.green < 255)
    {
      pick_color.green ++;
    }
    else
    {
      if (pick_color.blue < 255)
      {
        pick_color.blue ++;
      }
      else
      {
        pick_color.red = 0;
        pick_color.green = 0;
        pick_color.blue = 0;
        pick_color.alpha = 255;
      }
    }
  }

  top_level->widget.children_head = NULL;
  top_level->widget.children_tail = NULL;
  top_level->widget.next_sibling = NULL;

  top_level->widget.requested_size.width = 320;
  top_level->widget.requested_size.height = 240;

  ei_rect_t screen_location;
  screen_location.top_left.x = 0;
  screen_location.top_left.y = 0;
  screen_location.size.width = 320;
  screen_location.size.height = 240;
  top_level->widget.screen_location = screen_location;
  top_level->widget.content_rect = &screen_location;

  top_level->widget.placer_params = calloc(1, sizeof(ei_placer_params_t));

  top_level->widget.placer_params->anchor = NULL;
  top_level->widget.placer_params->anchor_data = ei_anc_northwest;
  top_level->widget.placer_params->x = NULL;
  top_level->widget.placer_params->x_data = 0;
  top_level->widget.placer_params->y = NULL;
  top_level->widget.placer_params->y_data = 0;
  top_level->widget.placer_params->w = NULL;
  top_level->widget.placer_params->w_data = 320;
  top_level->widget.placer_params->h = NULL;
  top_level->widget.placer_params->h_data = 240;
  top_level->widget.placer_params->rx = NULL;
  top_level->widget.placer_params->rx_data = 0.0;
  top_level->widget.placer_params->ry = NULL;
  top_level->widget.placer_params->ry_data = 0.0;
  top_level->widget.placer_params->rw = NULL;
  top_level->widget.placer_params->rw_data = 0.0;
  top_level->widget.placer_params->rh = NULL;
  top_level->widget.placer_params->rh_data = 0.0;

  top_level->title = "TopLevel";
  top_level->closable = EI_TRUE;
  top_level->resizable = ei_axis_both;
  top_level->min_size = malloc(sizeof(ei_size_t));
  top_level->min_size->width = 160;
  top_level->min_size->height = 120;
  top_level->color = ei_default_background_color;
  top_level->border_width = 4;
  top_level->carre_dim_actif = false;
  top_level->bouton_croix_actif = false;
  top_level->bordure_haut_actif = false;
}


void top_level_geomnotify(struct ei_widget_t*	widget,
							 ei_rect_t		rect)
{}


ei_bool_t top_level_handle (struct ei_widget_t*	widget,
						 struct ei_event_t*	event)
{
  ei_toplevel_widget_t *top_level = widget;
  ei_mouse_event_t souris = event->param.mouse;
  ei_point_t coin_dim_apres;


  /* SI ON ENFONCE LE BOUTON DE LA SOURIS */
  if (event->type == ei_ev_mouse_buttondown)
  {
    // si on est dans le carré de redimension, on active ce carré
    if (souris_dans_carre_redim(top_level, souris) && (top_level->carre_dim_actif == false))
    {
      // on retient la position de la souris au moment du clic
      coin_dim_avant = event->param.mouse.where;
      top_level->carre_dim_actif = true;
    }
    // si on est dans la bordure du haut, on active cette barre
    if (souris_dans_bordure(top_level, souris) && !(souris_dans_bouton_croix(top_level, souris)) && (top_level->bordure_haut_actif == false))
    {
      // on retient la position de la souris au moment du clic
      coin_dim_avant = event->param.mouse.where;
      top_level->bordure_haut_actif = true;
    }
    // si on est dans le bouton, on appuie dessus
    if (souris_dans_bouton_croix(top_level, souris) && (top_level->bouton_croix_actif == false))
    {
      top_level->bouton_croix_actif = true;
    }
  }

  /* SI ON BOUGE LA SOURIS */
  if (event->type == ei_ev_mouse_move)
  {
    // si carre de redimension active
    if (top_level->carre_dim_actif)
    {
      // on retient la position de la souris au moment du mouvement
      coin_dim_apres = event->param.mouse.where;
      //si on peut redimensionner uniquement selon x
      if (top_level->resizable == ei_axis_x)
      {
        // définition des nouvelles dimensions
        int new_width, new_height;
        new_width  = coin_dim_apres.x - top_level->widget.screen_location.top_left.x;
        new_height = coin_dim_avant.y - top_level->widget.screen_location.top_left.y;
        ei_place(widget, NULL, &(top_level->widget.screen_location.top_left.x), &(top_level->widget.screen_location.top_left.y), &new_width, &new_height, NULL, NULL, NULL, NULL);
      }
      //si on peut redimensionner uniquement selon y
      if (top_level->resizable == ei_axis_y)
      {
        // définition des nouvelles dimensions
        int new_width, new_height;
        new_width  = coin_dim_avant.x - top_level->widget.screen_location.top_left.x;
        new_height = coin_dim_apres.y - top_level->widget.screen_location.top_left.y;
        ei_place(widget, NULL, &(top_level->widget.screen_location.top_left.x), &(top_level->widget.screen_location.top_left.y), &new_width, &new_height, NULL, NULL, NULL, NULL);
      }
      //si on peut redimensionner selon x et y
      if (top_level->resizable == ei_axis_both)
      {
        // définition des nouvelles dimensions
        int new_width, new_height;
        new_width  = coin_dim_apres.x - top_level->widget.screen_location.top_left.x;
        new_height = coin_dim_apres.y - top_level->widget.screen_location.top_left.y;
        ei_place(widget, NULL, &(top_level->widget.screen_location.top_left.x), &(top_level->widget.screen_location.top_left.y), &new_width, &new_height, NULL, NULL, NULL, NULL);
      }
      draw_event = EI_TRUE;
    }

    // si barre du haut activée
    if (top_level->bordure_haut_actif)
    {
      // on retient la position de la souris au moment du déclic et on déplace top_level
      coin_dim_apres = event->param.mouse.where;
      int decallage_x = coin_dim_apres.x - coin_dim_avant.x;
      int decallage_y = coin_dim_apres.y - coin_dim_avant.y;
      top_level->widget.screen_location.top_left.x = top_level->widget.screen_location.top_left.x + decallage_x;
      top_level->widget.screen_location.top_left.y = top_level->widget.screen_location.top_left.y + decallage_y;

      int new_width, new_height;
      new_width = top_level->widget.screen_location.size.width;
      new_height = top_level->widget.screen_location.size.height;

      ei_place(widget, NULL, &(top_level->widget.screen_location.top_left.x), &(top_level->widget.screen_location.top_left.y), &new_width, &new_height, NULL, NULL, NULL, NULL);
    }

    // si le bouton est appuyé et on n'est plus dans les limites du bouton
    if (top_level->bouton_croix_actif && !(souris_dans_bouton_croix(top_level, souris)))
    {
      // on relâche le bouton
      top_level->bouton_croix_actif = false;
    }
    draw_event = EI_TRUE;
  }

  /* SI ON RELACHE LA SOURIS */
  if (event->type == ei_ev_mouse_buttonup)
  {
    // si carre de redimension activé, on le desactive
    if (top_level->carre_dim_actif)
    {
      top_level->carre_dim_actif = false;
    }
    // si barre du haut activée, on la désactive
    if (top_level->bordure_haut_actif)
    {
      top_level->bordure_haut_actif = false;
    }
    // si le bouton est appuyé, on le relâche
    if (top_level->bouton_croix_actif)
    {
      top_level->bouton_croix_actif = false;
      ei_widget_destroy(widget);
    }

  }
}

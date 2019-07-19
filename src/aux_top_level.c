#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../include/aux_point.h"
#include "../include/aux_bouton.h"
#include "../include/ei_draw.h"
#include "../include/ei_event.h"
#include "../include/ei_widget.h"
#include "../include/ei_bouton.h"
#include "../include/ei_top_level.h"
#include "../include/aux_top_level.h"


ei_bool_t souris_dans_carre_redim(ei_toplevel_widget_t *top_level, ei_mouse_event_t position_souris)
{

  int bordure_haut = 22 + 2*top_level->border_width;
  ei_rect_t grand_rect;
  grand_rect.top_left.x = top_level->widget.screen_location.top_left.x - top_level->border_width;
  grand_rect.top_left.y = top_level->widget.screen_location.top_left.y - bordure_haut;

  grand_rect.size.width = top_level->widget.screen_location.size.width + 2*top_level->border_width;
  grand_rect.size.height = top_level->widget.screen_location.size.height + bordure_haut + top_level->border_width;

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

  ei_point_t position = position_souris.where;
  return (is_in_clipper(position, &carre_redimensionnement));
}



ei_bool_t souris_dans_bordure(ei_toplevel_widget_t *top_level, ei_mouse_event_t position_souris)
{
  int bordure_haut = 22 + 2*top_level->border_width;
  ei_rect_t grand_rect;
  grand_rect.top_left.x = top_level->widget.screen_location.top_left.x - top_level->border_width;
  grand_rect.top_left.y = top_level->widget.screen_location.top_left.y - bordure_haut;

  grand_rect.size.width = top_level->widget.screen_location.size.width + 2*top_level->border_width;
  grand_rect.size.height = bordure_haut;

  ei_point_t position = position_souris.where;
  return (is_in_clipper(position, &grand_rect));
}


ei_bool_t souris_dans_bouton_croix(ei_toplevel_widget_t *top_level, ei_mouse_event_t position_souris)
{
  ei_rect_t rect_bouton;
  rect_bouton.top_left.x = top_level->widget.screen_location.top_left.x;
  rect_bouton.top_left.y = top_level->widget.screen_location.top_left.y - top_level->border_width - 20;
  rect_bouton.size.width = 20;
  rect_bouton.size.height = 20;

  ei_point_t position = position_souris.where;
  return (is_in_clipper(position, &rect_bouton));
}

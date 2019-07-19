#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../include/ei_types.h"
#include "../include/ei_widget.h"
#include "../include/ei_application.h"
#include "../include/hw_interface.h"
#include "../include/ei_draw.h"
#include "../include/ei_event.h"
#include "../include/ei_picking.h"
#include "../include/ei_bouton.h"
#include "../include/ei_frame.h"
#include "../include/ei_top_level.h"

ei_linked_widget_t *front_widget;
ei_widget_t *active_widget;
int pick_id = 0;
ei_color_t pick_color = {0,0,0,255};

extern ei_default_handle_func_t default_handle_func;
extern ei_surface_t pick_surface;
extern ei_surface_t main_window;

// renvoie si deux couleurs sont égales
bool are_colors_equal(ei_color_t color1, ei_color_t color2)
{
  return (color1.red == color2.red) && (color1.green == color2.green) && (color1.blue == color2.blue);
}

/* renvoie le widget situé à la position position de la surface surface
si aucun widget n'est situé à cette position: renvoie NULL */

void ei_handle_event(ei_event_t *event)
{
  if (event->type == ei_ev_mouse_move || event->type == ei_ev_mouse_buttonup || event->type == ei_ev_mouse_buttondown)
  {
    ei_widget_t *widget_under_mouse = NULL;
    widget_under_mouse = ei_widget_pick(&(event->param.mouse.where));
    if (event->type == ei_ev_mouse_buttondown)
    {
      ei_event_set_front_widget(widget_under_mouse);
      ei_event_set_active_widget(widget_under_mouse);
    }
    ei_widget_t *widget_to_handle = active_widget;
    widget_to_handle->wclass->handlefunc(widget_to_handle, event);
  }
  else
  {
    default_handle_func(event);
  }
}


ei_bool_t souris_dans_bouton(ei_bouton_widget_t *bouton, ei_mouse_event_t position_souris)
{
  ei_bool_t dedans = false;
  int x_min = bouton->widget.screen_location.top_left.x;
  int x_max = bouton->widget.screen_location.top_left.x + bouton->widget.screen_location.size.width;
  int y_min = bouton->widget.screen_location.top_left.y;
  int y_max = bouton->widget.screen_location.top_left.y + bouton->widget.screen_location.size.height;
  if ((position_souris.where.x <= x_max) && (position_souris.where.x >= x_min) && (position_souris.where.y >= y_min) && (position_souris.where.y <= y_max))
  {
    dedans = true;
  }
  return dedans;
}


void ei_draw_all_widgets(ei_surface_t surface, ei_surface_t pick_surface, ei_linked_widget_t *liste_widgets, ei_rect_t *clipper)
{
  if (liste_widgets->next == NULL)
  {
    liste_widgets->widget->wclass->drawfunc(liste_widgets->widget, surface, pick_surface, clipper);
  }
  else
  {
    ei_draw_all_widgets(surface, pick_surface, liste_widgets->next, clipper);
    liste_widgets->widget->wclass->drawfunc(liste_widgets->widget, surface, pick_surface, clipper);
  }
}


bool are_widgetclass_names_equal(ei_widgetclass_name_t name1, ei_widgetclass_name_t name2)
{
  char *name1_char = ei_widgetclass_stringname(name1);
  char *name2_char = ei_widgetclass_stringname(name2);

  while (*name1_char != '\0')
  {
    if (*name1_char != *name2_char)
    {
      return false;
    }
    name1_char ++;
    name2_char ++;
  }
  if (*name2_char != '\0')
  {
    return false;
  }
  return true;
}


void ei_event_set_front_widget(ei_widget_t *widget)
{
  if (widget != ei_app_root_widget() && front_widget->widget != widget)
  {
    ei_linked_widget_t *current_widget = front_widget;
    ei_linked_widget_t *next_widget = front_widget->next;

    while (next_widget->widget != widget)
    {
      current_widget = next_widget;
      next_widget = next_widget->next;
    }
    current_widget->next = next_widget->next;
    next_widget->next = front_widget;
    front_widget = next_widget;
  }
}

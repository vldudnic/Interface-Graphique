#include "../include/hw_interface.h"
#include "../include/ei_draw.h"
#include "../include/ei_widgetclass.h"
#include "../include/ei_widget.h"
#include "../include/ei_bouton.h"
#include "../include/aux_bouton.h"
#include "../include/ei_event.h"
#include "../include/ei_picking.h"
#include <stdbool.h>
#include <stdlib.h>


ei_widgetclass_t ei_widgetclass_bouton = {"button", &bouton_alloc, &bouton_release, &bouton_draw, &bouton_setdefault, &bouton_geomnotify, &bouton_handle};
extern ei_widgetclass_t *widgetclass_list;
extern ei_color_t pick_color;
extern int pick_id;
extern ei_bool_t draw_event;


void* bouton_alloc()
{
  return calloc(1, sizeof(ei_bouton_widget_t));
}


void bouton_release(struct ei_widget_t* widget)
{
  ei_bouton_widget_t *bouton = widget;
  ei_widget_t *first_child = bouton->widget.children_head;
  while (first_child != NULL)
  {
    ei_widget_t *temp = first_child;
    first_child = first_child->next_sibling;
    temp->wclass->releasefunc(temp);
  }
  free(bouton->img);
  free(bouton->user_param);
}

void bouton_draw(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper)
{
  ei_bouton_widget_t *bouton = widget;
  // si bouton relevé
  ei_draw_button_shape(surface, bouton->widget.screen_location, bouton->corner_radius, bouton->border_width, bouton->color, bouton->relief , clipper);

  if (bouton->img != NULL)
  {
    ei_size_t img_size = (bouton->img_rect != NULL) ? bouton->img_rect->size : hw_surface_get_size(bouton->img);
    ei_rect_t dest_rect;
    dest_rect.top_left = where_to_place(img_size, bouton->widget.screen_location, bouton->img_anchor);
    dest_rect.size = img_size;
    ei_copy_surface(surface, &dest_rect, bouton->img, bouton->img_rect, false);
  }
  if (bouton->text != NULL)
  {
    ei_size_t text_size;
    hw_text_compute_size(bouton->text, bouton->text_font, &(text_size.width), &(text_size.height));

    ei_point_t where = where_to_place(text_size, bouton->widget.screen_location, bouton->text_anchor);
    ei_draw_text(surface, &where, bouton->text, bouton->text_font, &(bouton->text_color), NULL);
  }

  ei_draw_button_shape(pick_surface, bouton->widget.screen_location, bouton->corner_radius, bouton->border_width, *(bouton->widget.pick_color), ei_relief_none , clipper);
}


void bouton_setdefault(struct ei_widget_t* widget)
{

  ei_bouton_widget_t *bouton = widget;

  bouton->widget.wclass = &ei_widgetclass_bouton;
  bouton->widget.pick_id = pick_id;
  pick_id ++;

  ei_color_t *new_pick_color = malloc(sizeof(ei_color_t));
  *new_pick_color = pick_color;
  bouton->widget.pick_color = new_pick_color;

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

  bouton->widget.children_head = NULL;
  bouton->widget.children_tail = NULL;
  bouton->widget.next_sibling = NULL;

  bouton->widget.requested_size.width = 0;
  bouton->widget.requested_size.height = 0;

  bouton->widget.placer_params = calloc(1, sizeof(ei_placer_params_t));

  bouton->widget.placer_params->anchor = NULL;
  bouton->widget.placer_params->anchor_data = ei_anc_northwest;
  bouton->widget.placer_params->x = NULL;
  bouton->widget.placer_params->x_data = 0;
  bouton->widget.placer_params->y = NULL;
  bouton->widget.placer_params->y_data = 0;
  bouton->widget.placer_params->w = NULL;
  bouton->widget.placer_params->w_data = 0;
  bouton->widget.placer_params->h = NULL;
  bouton->widget.placer_params->h_data = 0;
  bouton->widget.placer_params->rx = NULL;
  bouton->widget.placer_params->rx_data = 0.0;
  bouton->widget.placer_params->ry = NULL;
  bouton->widget.placer_params->ry_data = 0.0;
  bouton->widget.placer_params->rw = NULL;
  bouton->widget.placer_params->rw_data = 0.0;
  bouton->widget.placer_params->rh = NULL;
  bouton->widget.placer_params->rh_data = 0.0;

  ei_rect_t screen_location;
  screen_location.top_left.x = 0;
  screen_location.top_left.y = 0;
  screen_location.size.width = 0;
  screen_location.size.height = 0;
  bouton->widget.screen_location = screen_location;
  bouton->widget.content_rect = &screen_location;

  bouton->color = ei_default_background_color;
  bouton->border_width = 0;
  bouton->relief = ei_relief_raised;
  bouton->text = NULL;
  bouton->text_font = ei_default_font;
  bouton->text_color = ei_font_default_color;
  bouton->text_anchor = ei_anc_center;
  bouton->img = NULL;
  bouton->img_rect = NULL;
  bouton->img_anchor = ei_anc_center;
  bouton->corner_radius = k_default_button_corner_radius;
  bouton->callback = NULL;
  bouton->user_param = NULL;
}

void bouton_geomnotify(struct ei_widget_t*	widget,
							 ei_rect_t		rect)
{}


  ei_bool_t bouton_handle (struct ei_widget_t*	widget,
  						 struct ei_event_t*	event)
  {
    ei_bouton_widget_t *bouton = widget;
    if ((event->type == ei_ev_mouse_buttondown) && (bouton->relief == ei_relief_raised))
    {
      bouton->relief = ei_relief_sunken;
      //appeler callback de widget en lui passant user_param en parametres
      bouton->callback(widget, event, bouton->user_param);
      draw_event = EI_TRUE;
    }
    if ((event->type == ei_ev_mouse_buttonup) && (bouton->relief == ei_relief_sunken))
    {
      bouton->relief = ei_relief_raised;
      draw_event = EI_TRUE;
    }
    if ((bouton->relief == ei_relief_sunken) && (event->type == ei_ev_mouse_move))
    {
      ei_mouse_event_t souris = event->param.mouse;
      if (!(souris_dans_bouton(bouton, souris)))
      {
        bouton->relief = ei_relief_raised;
        draw_event = EI_TRUE;
      }
    }

  }

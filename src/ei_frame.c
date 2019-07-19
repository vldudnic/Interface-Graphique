#include "../include/hw_interface.h"
#include "../include/ei_draw.h"
#include "../include/ei_widgetclass.h"
#include "../include/ei_widget.h"
#include "../include/ei_frame.h"
#include "../include/aux_bouton.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

extern int pick_id;
extern ei_color_t pick_color;

ei_widgetclass_t ei_widgetclass_frame =
{"frame", &frame_alloc, &frame_release, &frame_draw, &frame_setdefault, &frame_geomnotify, &frame_handle};



void* frame_alloc()
{
  return calloc(1, sizeof(ei_frame_widget_t));
}


void frame_release(struct ei_widget_t* widget)
{
  ei_frame_widget_t *frame = widget;
  ei_widget_t *first_child = frame->widget.children_head;
  while (first_child != NULL)
  {
    ei_widget_t *temp = first_child;
    first_child = first_child->next_sibling;
    temp->wclass->releasefunc(temp);
  }
  free(frame->text);
  free(frame->img);
  free(frame->img_rect);
}



void frame_draw(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper)
{
  ei_frame_widget_t *frame = widget;
  
  ei_draw_button_shape(surface, frame->widget.screen_location, 0, frame->border_width, frame->color, ei_relief_raised , clipper);

  if (frame->img != NULL)
  {
    ei_size_t img_size = (frame->img_rect != NULL) ? frame->img_rect->size : hw_surface_get_size(frame->img);
    ei_rect_t dest_rect;
    dest_rect.top_left = where_to_place(img_size, frame->widget.screen_location, frame->img_anchor);
    dest_rect.size = img_size;
    ei_copy_surface(surface, &dest_rect, frame->img, frame->img_rect, false);
  }
  if (frame->text != NULL)
  {
    ei_size_t text_size;
    hw_text_compute_size(frame->text, frame->text_font, &(text_size.width), &(text_size.height));

    ei_point_t where = where_to_place(text_size, frame->widget.screen_location, frame->text_anchor);
    ei_draw_text(surface, &where, frame->text, frame->text_font, &(frame->text_color), NULL);
  }

  ei_draw_button_shape(pick_surface, frame->widget.screen_location, 0, frame->border_width, *(frame->widget.pick_color), ei_relief_none , clipper);
}




void frame_setdefault(struct ei_widget_t* widget)
{
  ei_frame_widget_t *frame = widget;

  frame->widget.wclass = &ei_widgetclass_frame;
  frame->widget.pick_id = pick_id;
  pick_id ++;
  ei_color_t *new_pick_color = malloc(sizeof(ei_color_t));
  *new_pick_color = pick_color;
  frame->widget.pick_color = new_pick_color;
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

  frame->widget.children_head = NULL;
  frame->widget.children_tail = NULL;
  frame->widget.next_sibling = NULL;

  frame->widget.requested_size.width = 0;
  frame->widget.requested_size.height = 0;

  ei_rect_t screen_location;
  screen_location.top_left.x = 0;
  screen_location.top_left.y = 0;
  screen_location.size.width = 0;
  screen_location.size.height = 0;
  frame->widget.screen_location = screen_location;
  frame->widget.content_rect = &screen_location;

  frame->widget.placer_params = calloc(1, sizeof(ei_placer_params_t));

  frame->widget.placer_params->anchor = NULL;
  frame->widget.placer_params->anchor_data = ei_anc_northwest;
  frame->widget.placer_params->x = NULL;
  frame->widget.placer_params->x_data = 0;
  frame->widget.placer_params->y = NULL;
  frame->widget.placer_params->y_data = 0;
  frame->widget.placer_params->w = NULL;
  frame->widget.placer_params->w_data = 0;
  frame->widget.placer_params->h = NULL;
  frame->widget.placer_params->h_data = 0;
  frame->widget.placer_params->rx = NULL;
  frame->widget.placer_params->rx_data = 0.0;
  frame->widget.placer_params->ry = NULL;
  frame->widget.placer_params->ry_data = 0.0;
  frame->widget.placer_params->rw = NULL;
  frame->widget.placer_params->rw_data = 0.0;
  frame->widget.placer_params->rh = NULL;
  frame->widget.placer_params->rh_data = 0.0;

  frame->color = ei_default_background_color;
  frame->border_width = 0;
  frame->relief = ei_relief_none;
  frame->text = NULL;
  frame->text_font = ei_default_font;
  frame->text_color = ei_font_default_color;
  frame->text_anchor = ei_anc_center;
  frame->img = NULL;
  frame->img_rect = NULL;
  frame->img_anchor = ei_anc_center;
}

void frame_geomnotify(struct ei_widget_t*	widget,
							 ei_rect_t		rect)
{}


ei_bool_t frame_handle(struct ei_widget_t*	widget,
						 struct ei_event_t*	event)
{

}

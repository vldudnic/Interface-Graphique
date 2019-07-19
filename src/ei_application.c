#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../include/ei_types.h"
#include "../include/ei_widget.h"
#include "../include/ei_application.h"
#include "../include/hw_interface.h"
#include "../include/ei_draw.h"
#include "../include/ei_event.h"
#include "../include/ei_frame.h"
#include "../include/ei_bouton.h"
#include "../include/ei_top_level.h"
#include "../include/ei_picking.h"

ei_linked_widget_t *front_widget = NULL;
ei_widget_t *active_widget = NULL;
ei_surface_t main_window = NULL;
ei_surface_t pick_surface = NULL;
ei_bool_t run;
ei_bool_t draw_event = EI_TRUE;



void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen)
{
  hw_init();
	main_window = hw_create_window(main_window_size, fullscreen);
  pick_surface = hw_surface_create(main_window, main_window_size, EI_TRUE);

  ei_frame_register_class();
  ei_button_register_class();
  ei_toplevel_register_class();

  ei_widget_t *root_widget = ei_widget_create("frame", NULL);
  active_widget = root_widget;
  ei_frame_configure(root_widget, main_window_size, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}


void ei_app_run()
{
  run = true;
  while (run)
  {
    if (draw_event)
    {
      hw_surface_lock(main_window);
      hw_surface_lock(pick_surface);

      ei_draw_all_widgets(main_window, pick_surface, front_widget, NULL);

      hw_surface_unlock(main_window);
      hw_surface_unlock(pick_surface);

      hw_surface_update_rects(main_window, NULL);
      hw_surface_update_rects(pick_surface, NULL);

      draw_event = EI_FALSE;
    }

    ei_event_t *current_event = malloc(sizeof(ei_event_t));
    hw_event_wait_next(current_event);
    ei_handle_event(current_event);
    free(current_event);
  }
}

void ei_app_free()
{
  while (front_widget != NULL)
  {
    ei_linked_widget_t *temp = front_widget;
    front_widget = front_widget->next;
    temp->widget->wclass->releasefunc(temp->widget);
    free(temp);
  }
  hw_quit();
}

void ei_app_invalidate_rect(ei_rect_t* rect)
{}


void ei_app_quit_request()
{
  run = false;
}


ei_widget_t* ei_app_root_widget()
{
  ei_widget_t *widget = front_widget->widget;
  while (widget->parent != NULL)
  {
    widget = widget->parent;
  }
  return widget;
}


ei_surface_t ei_app_root_surface()
{

}

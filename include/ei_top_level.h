#ifndef EI_TOP_LEVEL_H
#define EI_TOP_LEVEL_H

#include <stdlib.h>
#include <stdbool.h>


#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_widgetclass.h"
#include "ei_types.h"
#include "ei_widget.h"


/* tout ce qui concerne les top levels */


/*
definition du widget toplevel
*/
typedef struct ei_toplevel_widget_t
{
  ei_widget_t widget;
  char* title;
  ei_bool_t closable;
  ei_axis_set_t resizable;
  ei_size_t* min_size;
  ei_color_t color;
  int border_width;
  bool carre_dim_actif;
  bool bouton_croix_actif;
  bool bordure_haut_actif;
} ei_toplevel_widget_t;

/* allocfunc */
void* top_level_alloc();

/*releasefunc */
void top_level_release(struct ei_widget_t* widget);


/* drawfunc */
void top_level_draw(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper);

/* setdefaultfunc */
void top_level_setdefault(struct ei_widget_t* widget);

/* geomnotifyfunc */
void top_level_geomnotify(struct ei_widget_t*	widget,
							 ei_rect_t		rect);

/* handlefunc */
ei_bool_t top_level_handle(struct ei_widget_t*	widget,
						 struct ei_event_t*	event);

#endif

#ifndef EI_FRAME_H
#define EI_FRAME_H

#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_widgetclass.h"
#include "ei_types.h"
#include "ei_widget.h"

/*
tout ce qui concerne les widgets frame
*/


/*
definition du widget frame
*/
typedef struct ei_frame_widget_t
{
  ei_widget_t widget;
  ei_relief_t relief;
  char* text;
	int border_width;
	ei_color_t color;
  ei_font_t text_font;
  ei_color_t text_color;
	ei_anchor_t text_anchor;
	ei_surface_t img;
	ei_rect_t* img_rect;
	ei_anchor_t img_anchor;
} ei_frame_widget_t;

/* allocfunc */
void* frame_alloc();

/*releasefunc */
void frame_release(struct ei_widget_t* widget);


/* drawfunc */
void frame_draw(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper);

/* setdefaultfunc */
void frame_setdefault(struct ei_widget_t* widget);

/* geomnotifyfunc */
void frame_geomnotify(struct ei_widget_t*	widget,
							 ei_rect_t		rect);

/* handlefunc */
ei_bool_t frame_handle(struct ei_widget_t*	widget,
						 struct ei_event_t*	event);


#endif

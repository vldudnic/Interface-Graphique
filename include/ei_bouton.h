#ifndef EI_BOUTON_H
#define EI_BOUTON_H

#include "../include/hw_interface.h"
#include "../include/ei_draw.h"
#include "../include/ei_widgetclass.h"
#include "../include/ei_widget.h"


/*
tout ce qui concerne les widgets button
*/


/*
definition du widget bouton
*/
typedef struct ei_bouton_widget_t
{
	ei_widget_t widget;
	ei_color_t color;
	int border_width;
	int corner_radius;
	ei_relief_t	relief;
	char*	text;
	ei_font_t	text_font;
	ei_color_t text_color;
	ei_anchor_t text_anchor;
	ei_surface_t	img;
	ei_rect_t*	img_rect;
	ei_anchor_t	img_anchor;
	ei_callback_t	callback;
	void*	user_param;
} ei_bouton_widget_t;


/* allocfunc */
void* bouton_alloc();

/*releasefunc */
void bouton_release(struct ei_widget_t* widget);


/* drawfunc */
void bouton_draw(struct ei_widget_t*	widget,
							 ei_surface_t		surface,
							 ei_surface_t		pick_surface,
							 ei_rect_t*		clipper);

/* setdefaultfunc */
void bouton_setdefault(struct ei_widget_t* widget);

/* geomnotifyfunc */
void bouton_geomnotify(struct ei_widget_t*	widget,
							 ei_rect_t		rect);

/* handlefunc */
ei_bool_t bouton_handle(struct ei_widget_t*	widget,
						 struct ei_event_t*	event);

#endif

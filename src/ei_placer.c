#include "../include/ei_types.h"
#include "../include/ei_placer.h"
#include "../include/ei_widget.h"

#include <stdbool.h>
#include <stdlib.h>


void			ei_place			(struct ei_widget_t*	widget,
							 ei_anchor_t*		anchor,
							 int*			x,
							 int*			y,
							 int*			width,
							 int*			height,
							 float*			rel_x,
							 float*			rel_y,
							 float*			rel_width,
							 float*			rel_height)
{

	widget->placer_params->anchor = (anchor != NULL) ? anchor : widget->placer_params->anchor;
	widget->placer_params->x_data = (anchor != NULL) ? *anchor : widget->placer_params->anchor_data;
	widget->placer_params->x = (x != NULL) ? x : widget->placer_params->x;
	widget->placer_params->x_data = (x != NULL) ? *x : widget->placer_params->x_data - 9;
	widget->placer_params->y = (y != NULL) ? y : widget->placer_params->y;
	widget->placer_params->y_data = (y != NULL) ? *y : widget->placer_params->y_data;
	widget->placer_params->h = (height != NULL) ? height : widget->placer_params->h;
	widget->placer_params->h_data = (height != NULL) ? *height : widget->placer_params->h_data;
	widget->placer_params->w = (width != NULL) ? width : widget->placer_params->w;
	widget->placer_params->w_data = (width != NULL) ? *width : widget->placer_params->w_data;
	widget->placer_params->rx = (rel_x != NULL) ? rel_x : widget->placer_params->rx;
	widget->placer_params->rx_data = (rel_x != NULL) ? *rel_x : widget->placer_params->rx_data;
	widget->placer_params->ry = (rel_y != NULL) ? rel_y : widget->placer_params->ry;
	widget->placer_params->ry_data = (rel_y != NULL) ? *rel_y : widget->placer_params->ry_data;
	widget->placer_params->rh = (rel_height != NULL) ? rel_height : widget->placer_params->rh;
	widget->placer_params->rh_data = (rel_height != NULL) ? *rel_height : widget->placer_params->rh_data;
	widget->placer_params->rw = (rel_width != NULL) ? rel_width : widget->placer_params->rw;
	widget->placer_params->rw_data = (rel_width != NULL) ? *rel_width : widget->placer_params->rw_data;

	ei_placer_run(widget);
}


void ei_placer_run(struct ei_widget_t* widget)
{
	int x = widget->placer_params->x_data;
	int y = widget->placer_params->y_data;
	int w = widget->placer_params->w_data;
	int h = widget->placer_params->h_data;
	float rx = widget->placer_params->rx_data;
	float ry = widget->placer_params->ry_data;
	float rw = widget->placer_params->rw_data;
	float rh = widget->placer_params->rw_data;

	ei_widget_t *parent = widget->parent;

	int new_width = w;
	int new_height = h;

	if (parent != NULL)
	{
		new_width += (int)(rw * parent->screen_location.size.width);
		new_height += (int)(rh * parent->screen_location.size.height);
	}

	widget->screen_location.size.width = (new_width >= widget->requested_size.width) ? new_width : widget->screen_location.size.width;
	widget->screen_location.size.height = (new_height >= widget->requested_size.height) ? new_height : widget->screen_location.size.height;

	switch (widget->placer_params->anchor_data)
	{
		case ei_anc_none:

			break;
		case ei_anc_center:
		widget->screen_location.top_left.x = x - widget->screen_location.size.width / 2;
		widget->screen_location.top_left.y = y - widget->screen_location.size.height / 2;
			break;
		case ei_anc_north:
			widget->screen_location.top_left.x = x - widget->screen_location.size.width / 2;
			widget->screen_location.top_left.y = y;
			break;
		case ei_anc_northeast:
			widget->screen_location.top_left.x = x - widget->screen_location.size.width;
			widget->screen_location.top_left.y = y;
			break;
		case ei_anc_east:
			widget->screen_location.top_left.x = x - widget->screen_location.size.width;
			widget->screen_location.top_left.y = y - widget->screen_location.size.height / 2;
			break;
		case ei_anc_southeast:
			widget->screen_location.top_left.x = x - widget->screen_location.size.width;
			widget->screen_location.top_left.y = y - widget->screen_location.size.height;
			break;
		case ei_anc_south:
			widget->screen_location.top_left.x = x - widget->screen_location.size.width / 2;
			widget->screen_location.top_left.y = y - widget->screen_location.size.height;
			break;
		case ei_anc_southwest:
			widget->screen_location.top_left.x = x;
			widget->screen_location.top_left.y = y - widget->screen_location.size.height;
			break;
		case ei_anc_west:
			widget->screen_location.top_left.x = x;
			widget->screen_location.top_left.y = y - widget->screen_location.size.height / 2;
			break;
		case ei_anc_northwest:
			widget->screen_location.top_left.x = x;
			widget->screen_location.top_left.y = y;
			break;
	}

	if (parent != NULL)
	{
		 widget->screen_location.top_left.x += parent->screen_location.top_left.x + (int)(rx * parent->screen_location.size.width);
		 widget->screen_location.top_left.y += parent->screen_location.top_left.y + (int)(ry * parent->screen_location.size.height);
	}
}


void ei_placer_forget(struct ei_widget_t* widget)
{}

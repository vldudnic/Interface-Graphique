#include "../include/ei_widgetclass.h"
#include "../include/ei_placer.h"
#include "../include/ei_widget.h"
#include "../include/ei_top_level.h"
#include "../include/ei_frame.h"
#include "../include/ei_bouton.h"
#include "../include/ei_picking.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

extern ei_widgetclass_t* widgetclass_list;
extern ei_linked_widget_t *front_widget;
extern ei_widget_t *active_widget;
extern ei_surface_t main_window;
extern ei_surface_t pick_surface;

extern int pick_id;
extern ei_color_t pick_color;


bool is_class_registered(ei_widgetclass_name_t	class_name);

bool is_class_registered(ei_widgetclass_name_t	class_name)
{
	ei_widgetclass_t *curseur = widgetclass_list;
	while (curseur != NULL)
	{
		if (are_widgetclass_names_equal(class_name, curseur->name))
		{
			return true;
		}
		curseur = curseur->next;
	}
	return false;
}



ei_widget_t*		ei_widget_create		(ei_widgetclass_name_t	class_name,
							 ei_widget_t*		parent)
{
	if (is_class_registered(class_name))
	{
		ei_widget_t *new_widget = NULL;
		ei_widgetclass_t *new_widget_class = ei_widgetclass_from_name(class_name);
		new_widget = new_widget_class->allocfunc();

		new_widget->wclass = new_widget_class;
		new_widget->parent = parent;
		new_widget->wclass->setdefaultsfunc(new_widget);

		if (new_widget->parent != NULL)
		{
			ei_widget_t *curseur_widget = new_widget->parent->children_head;
			if (curseur_widget == NULL)
			{
				new_widget->parent->children_head = new_widget;
			}
			else
			{
				while (curseur_widget->next_sibling != NULL)
				{
					curseur_widget = curseur_widget->next_sibling;
				}
				curseur_widget->next_sibling = new_widget;
			}
			new_widget->parent->children_tail = new_widget;
		}

		ei_linked_widget_t *new_linked_widget = malloc(sizeof(ei_linked_widget_t));
		new_linked_widget->widget = new_widget;
		new_linked_widget->next = front_widget;
		front_widget = new_linked_widget;

		return new_widget;
	}
	else
	{
		return NULL;
	}
}


void			ei_widget_destroy		(ei_widget_t*		widget)
{
	widget->wclass->releasefunc(widget);
}


ei_widget_t*		ei_widget_pick			(ei_point_t*		where)
{
	// on veut acceder à l'adresse mémoire du pixel à la position position
	uint8_t* p_adresse_origine_surface = hw_surface_get_buffer(pick_surface);
	ei_size_t taille_surface = hw_surface_get_size(pick_surface);
	uint8_t *p_point_RAM = malloc(sizeof(uint8_t));
	p_point_RAM = p_adresse_origine_surface + (where->y * taille_surface.width + where->x) * 4;

	int *ir = malloc(sizeof(int));
	int *ig = malloc(sizeof(int));
	int *ib = malloc(sizeof(int));
	int *ia = malloc(sizeof(int));

	hw_surface_get_channel_indices(pick_surface,  ir,  ig,  ib,  ia);

	ei_color_t color;
	uint8_t *p_point_r = p_point_RAM + *ir;
	uint8_t *p_point_g = p_point_RAM + *ig;
	uint8_t *p_point_b = p_point_RAM + *ib;
	uint8_t *p_point_a = p_point_RAM + *ia;

	color.red = *p_point_r;
	color.green = *p_point_g;
	color.blue = *p_point_b;
	color.alpha = *p_point_a;

	ei_linked_widget_t *current_widget = front_widget;

	while (current_widget != NULL)
	{
		if (are_colors_equal(*(current_widget->widget->pick_color), color))
		{
			return current_widget->widget;
		}
		current_widget = current_widget->next;
	}

	return NULL;
}


  void			ei_frame_configure		(ei_widget_t*		widget,
  							 ei_size_t*		requested_size,
  							 const ei_color_t*	color,
  							 int*			border_width,
  							 ei_relief_t*		relief,
  							 char**		text,
  							 ei_font_t*	 text_font,
  							 ei_color_t*		text_color,
  							 ei_anchor_t*		text_anchor,
  							 ei_surface_t*		img,
  							 ei_rect_t**		img_rect,
  							 ei_anchor_t*		img_anchor)
{

	ei_frame_widget_t *frame = widget;
	frame->widget.requested_size = (requested_size == NULL) ?  frame->widget.requested_size : *requested_size;
	frame->color = (color == NULL) ? frame->color : *color;
	frame->border_width = (border_width == NULL) ? frame->border_width : *border_width;
	frame->relief = (relief == NULL) ? frame->relief : *relief;
	frame->text = (text == NULL) ? frame->text : *text;
	frame->text_font = (text_font == NULL) ? frame->text_font : *text_font;
	frame->text_color = (text_color == NULL) ? frame->text_color : *text_color;
	frame->text_anchor = (text_anchor == NULL) ? frame->text_anchor : *text_anchor;
	frame->img = (img == NULL) ? frame->img : *img;
	frame->img_rect = (img_rect == NULL) ? frame->img_rect : *img_rect;
	frame->img_anchor = (img_anchor == NULL) ? frame->img_anchor : *img_anchor;


	ei_size_t new_requested_size;

	if (frame->text != NULL)
	{
		int *width = malloc(sizeof(int));
		int *height = malloc(sizeof(int));

		hw_text_compute_size (frame->text, frame->text_font, width, height);
 		new_requested_size.width = *width;
		new_requested_size.height = *height;
		frame->widget.requested_size = new_requested_size;

		free(width);
		free(height);
	}

	if (frame->img != NULL)
	{
		if (frame->img_rect != NULL)
		{
			new_requested_size = (*img_rect)->size;
		}
		else
		{
			new_requested_size = hw_surface_get_size(frame->img);
		}
		frame->widget.requested_size = new_requested_size;
	}

	frame->widget.screen_location.size = (requested_size != NULL) ? *requested_size : frame->widget.screen_location.size;

	if (frame->widget.requested_size.width > frame->widget.screen_location.size.width || frame->widget.requested_size.height > frame->widget.screen_location.size.height)
	{
		frame->widget.screen_location.size = frame->widget.requested_size;
	}
}



  void			ei_button_configure		(ei_widget_t*		widget,
  							 ei_size_t*		requested_size,
  							 const ei_color_t*	color,
  							 int*			border_width,
  							 int*			corner_radius,
  							 ei_relief_t*		relief,
  							 char**			text,
  							 ei_font_t*		text_font,
  							 ei_color_t*		text_color,
  							 ei_anchor_t*		text_anchor,
  							 ei_surface_t*		img,
  							 ei_rect_t**		img_rect,
  							 ei_anchor_t*		img_anchor,
  							 ei_callback_t*		callback,
  							 void**			user_param)
{
	ei_bouton_widget_t *bouton = widget;

	bouton->color = (color == NULL) ? bouton->color : *color;
	bouton->border_width = (border_width == NULL) ? bouton->border_width : *border_width;
	bouton->corner_radius = (corner_radius == NULL) ? bouton->corner_radius : *corner_radius;
	bouton->relief = (relief == NULL) ? bouton->relief : *relief;
	bouton->text = (text == NULL) ? bouton->text : *text;
	bouton->text_font = (text_font == NULL) ? bouton->text_font : *text_font;
	bouton->text_color = (text_color == NULL) ? bouton->text_color : *text_color;
	bouton->text_anchor = (text_anchor == NULL) ? bouton->text_anchor : *text_anchor;
	bouton->img = (img == NULL) ? bouton->img : *img;
	bouton->img_rect = (img_rect == NULL) ? bouton->img_rect : *img_rect;
	bouton->img_anchor = (img_anchor == NULL) ? bouton->img_anchor : *img_anchor;
	bouton->callback = (callback == NULL) ? bouton->callback : *callback;
	bouton->user_param = (user_param == NULL) ? bouton->user_param : *user_param;

	ei_size_t new_requested_size;

	if (bouton->text != NULL)
	{
		int *width = malloc(sizeof(int));
		int *height = malloc(sizeof(int));

		hw_text_compute_size (bouton->text, bouton->text_font, width, height);
 		new_requested_size.width = *width;
		new_requested_size.height = *height;
		bouton->widget.requested_size = new_requested_size;

		free(width);
		free(height);
	}

	if (bouton->img != NULL)
	{
		if (bouton->img_rect != NULL)
		{
			new_requested_size = (*img_rect)->size;
		}
		else
		{
			new_requested_size = hw_surface_get_size(bouton->img);
		}
		bouton->widget.requested_size = new_requested_size;
	}

	bouton->widget.screen_location.size = (requested_size != NULL) ? *requested_size : bouton->widget.screen_location.size;

	if (bouton->widget.requested_size.width > bouton->widget.screen_location.size.width || bouton->widget.requested_size.height > bouton->widget.screen_location.size.height)
	{
		bouton->widget.screen_location.size = bouton->widget.requested_size;
	}
}




  void			ei_toplevel_configure		(ei_widget_t*		widget,
  							 ei_size_t*		requested_size,
  							 ei_color_t*		color,
  							 int*			border_width,
  							 char**			title,
  							 ei_bool_t*		closable,
  							 ei_axis_set_t*		resizable,
  						 	 ei_size_t**		min_size)
{
	ei_toplevel_widget_t *top_level = widget;
	top_level->widget.requested_size = (requested_size == NULL) ? top_level->widget.requested_size : *requested_size;
	top_level->color = (color == NULL) ? top_level->color : *color;
	top_level->border_width = (border_width == NULL) ? top_level->border_width : *border_width;
	top_level->title = (title == NULL) ? top_level->title : *title;
	top_level->closable = (closable == NULL) ? top_level->closable : *closable;
	top_level->resizable = (resizable == NULL) ? top_level->resizable : *resizable;
	top_level->min_size = (min_size == NULL) ? top_level->min_size : *min_size;

	ei_place(widget, NULL, NULL, NULL, &(requested_size->width), &(requested_size->height), NULL, NULL, NULL, NULL);
}

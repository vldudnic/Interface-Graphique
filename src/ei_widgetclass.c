#include <stdio.h>
#include <stdlib.h>


#include "../include/ei_widgetclass.h"
#include "../include/ei_widget.h"
#include "../include/ei_picking.h"


ei_widgetclass_t *widgetclass_list = NULL;
extern ei_widgetclass_t ei_widgetclass_toplevel;
extern ei_widgetclass_t ei_widgetclass_frame;
extern ei_widgetclass_t ei_widgetclass_bouton;



void			ei_widgetclass_register		(ei_widgetclass_t* widgetclass)
{}



ei_widgetclass_t*	ei_widgetclass_from_name	(ei_widgetclass_name_t name)
{
  ei_widgetclass_t *curseur = widgetclass_list;
  char *name_char = ei_widgetclass_stringname(name);
  char *curseur_name_char = NULL;
  while (curseur != NULL)
  {
    if (are_widgetclass_names_equal(name, curseur->name))
    {
      return curseur;
    }
    curseur = curseur->next;
  }
  return curseur;
}


void			ei_frame_register_class 	()
{
  ei_widgetclass_frame.next = widgetclass_list;
  widgetclass_list = &ei_widgetclass_frame;
}


void			ei_button_register_class 	()
{
  ei_widgetclass_bouton.next = widgetclass_list;
  widgetclass_list = &ei_widgetclass_bouton;
}



void			ei_toplevel_register_class 	()
{
  ei_widgetclass_toplevel.next = widgetclass_list;
  widgetclass_list = &ei_widgetclass_toplevel;
}

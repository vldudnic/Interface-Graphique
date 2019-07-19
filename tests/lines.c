#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../include/hw_interface.h"
#include "../include/ei_utils.h"
#include "../include/ei_draw.h"
#include "../include/ei_types.h"
#include "../include/aux_point.h"
#include "../include/aux_polygon.h"
#include "../include/aux_bouton.h"

 #define M_PI 3.1415


/* test_line --
 *
 *	Draws a simple line in the canonical octant, that is, x1>x0 and y1>y0, with
 *	dx > dy. This can be used to test a first implementation of Bresenham
 *	algorithm, for instance.
 */
void test_line(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 255, 0, 255, 255 };
	ei_linked_point_t	pts[2];

	pts[0].point.x = 200; pts[0].point.y = 200; pts[0].next = &pts[1];
	pts[1].point.x = 600; pts[1].point.y = 400; pts[1].next = NULL;

	ei_draw_polyline(surface, pts, color, clipper);
}



/* test_octogone --
 *
 *	Draws an octogone in the middle of the screen. This is meant to test the
 *	algorithm that draws a polyline in each of the possible octants, that is,
 *	in each quadrant with dx>dy (canonical) and dy>dx (steep).
 */
void test_octogone(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 0, 255, 0, 255 };
	ei_linked_point_t	pts[9];
	int			i, xdiff, ydiff;

	/* Initialisation */
	pts[0].point.x = 400;
	pts[0].point.y = 90;

	/* Draw the octogone */
	for(i = 1; i <= 8; i++) {
		 /*	Add or remove 70/140 pixels for next point
			The first term of this formula gives the sign + or - of the operation
			The second term is 2 or 1, according to which coordinate grows faster
			The third term is simply the amount of pixels to skip */
		xdiff = pow(-1, (i + 1) / 4) * pow(2, (i / 2) % 2 == 0) * 70;
		ydiff = pow(-1, (i - 1) / 4) * pow(2, (i / 2) % 2) * 70;

		pts[i].point.x = pts[i-1].point.x + xdiff;
		pts[i].point.y = pts[i-1].point.y + ydiff;
		pts[i-1].next = &(pts[i]);
	}

	/* End the linked list */
	pts[i-1].next = NULL;

	/* Draw the form with polylines */
	ei_draw_polyline(surface, pts, color, clipper);
}



/* test_square --
 *
 *	Draws a square in the middle of the screen. This is meant to test the
 *	algorithm for the special cases of horizontal and vertical lines, where
 *	dx or dy are zero
 */
void test_square(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t color_red = { 255, 0, 0, 255 };
	ei_color_t color_green = {0,255,0,255};
	ei_color_t color_blue = {0,0,255,255};
  ei_color_t color = {100,100,100, 255};
	ei_linked_point_t	pts[5];

	pts[0].point.x = 100;
	pts[0].point.y = 100;
	pts[0].next = &pts[1];

	pts[1].point.x = 200;
	pts[1].point.y = 200;
	pts[1].next = &pts[2];

	pts[2].point.x = 300;
	pts[2].point.y = 100;
	pts[2].next = &pts[3];

	pts[3].point.x = 200;
	pts[3].point.y = 300;
	pts[3].next = &pts[4];

	pts[4].point.x = 100;
	pts[4].point.y = 100;
	pts[4].next = NULL;


	/* Draw the form with polylines */
	ei_draw_polyline(surface, pts, color_red, clipper);
	ei_draw_polygon(surface, pts, color_red, clipper);

	ei_point_t top_left = {500,500};
	ei_size_t taille = {0, 0};
	ei_rect_t rectangle_bouton = {top_left, taille};

 	ei_draw_button_shape(surface, rectangle_bouton, 8, 2, color, ei_relief_raised, clipper);
}




/* test_dot --
 *
 *	Draws a dot in the middle of the screen. This is meant to test the special
 *	case when dx = dy = 0
 */
void test_dot(ei_surface_t surface, ei_rect_t* clipper)
{
	ei_color_t		color		= { 0, 0, 0, 255 };
	ei_linked_point_t	pts[3];

	pts[0].point.x = 400; pts[0].point.y = 300; pts[0].next = &(pts[1]);
	pts[1].point.x = 400; pts[1].point.y = 300; pts[1].next = NULL;

	ei_draw_polyline(surface, pts, color, clipper);
}



/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
  ei_size_t		win_size	= ei_size(800, 800);
  ei_surface_t		main_window	= NULL;
  ei_color_t		white		= { 0xff, 0xff, 0xff, 0xff };
  ei_color_t blue = {0x00, 0x00, 0xff, 0xff};
  ei_color_t red = {0xff, 0x00, 0x00, 0xff};
  ei_rect_t*		clipper_ptr	= NULL;
  static char*	k_default_image_filename	= "misc/klimt.jpg";


  hw_init();
  main_window = hw_create_window(&win_size, EI_FALSE);

  ei_surface_t image = hw_image_load(k_default_image_filename, main_window);
  ei_rect_t rect_image = hw_surface_get_rect(image);
  ei_rect_t ou_copier;
  ou_copier.top_left.x = 0;
  ou_copier.top_left.y = 0;
  ou_copier.size = rect_image.size;


  /* Lock the drawing surface, paint it white. */
  hw_surface_lock	(main_window);

  ei_fill		(main_window, &white, NULL);
  hw_surface_lock (image);
  ei_point_t point = {100,100};


  ei_draw_text		(main_window, &point, "Salut", ei_default_font, &red, NULL);


  // ei_fill		(main_window, &white, clipper_ptr);
   ei_copy_surface(main_window, &ou_copier, image, NULL, false);

  //test_line	(main_window, clipper_ptr);
  test_octogone	(main_window, clipper_ptr);
  test_square	(main_window, clipper_ptr);
  //test_dot	(main_window, clipper_ptr);


   hw_surface_unlock(main_window);
   hw_surface_unlock (image);

   hw_surface_update_rects(main_window, NULL);

   getchar();

	hw_quit();
	return (EXIT_SUCCESS);
}

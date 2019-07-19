#include <stdio.h>
#include <stdlib.h>

#include "../include/ei_application.h"
#include "../include/ei_event.h"
#include "../include/hw_interface.h"
#include "../include/ei_widget.h"


/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size		= {700, 700};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_widget_t*	frame;
	ei_widget_t*	frame2;
	ei_size_t	frame_size		= {400,300};
	int		frame_x			= 150;
	int		frame_y			= 200;
	ei_color_t	frame_color		= {0x88, 0x88, 0x88, 0xff};
	ei_relief_t	frame_relief		= ei_relief_raised;
	int		frame_border_width	= 6;

	ei_size_t	frame_size2		= {50,50};
	int		frame_x2		= 50;
	int		frame_y2			= 50;
	ei_color_t	frame_color2	= {255, 0x00, 0x00, 0xff};
	ei_relief_t	frame_relief2		= ei_relief_raised;
	int		frame_border_width2	= 6;

	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	/* Create, configure and place the frame on screen. */
	frame = ei_widget_create("top_level", ei_app_root_widget());
	ei_toplevel_configure	(frame, &frame_size, &frame_color,
				 &frame_border_width, NULL, NULL, NULL, NULL);
	ei_place(frame, NULL, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL, NULL );


	frame2 = ei_widget_create("frame", frame);
	ei_frame_configure	(frame2, &frame_size2, &frame_color2,
				 &frame_border_width2, &frame_relief2, NULL, NULL, NULL, NULL,
				 NULL, NULL, NULL);
	ei_place(frame2, NULL, &frame_x2, &frame_y2, NULL, NULL, NULL, NULL, NULL, NULL );

	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}

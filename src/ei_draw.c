#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "../include/ei_draw.h"
#include "../include/aux_point.h"


/* Toutes les fonctions de dessin supposent qu'on a déjà fait appel aux fonctions:
	-hw_init()
	-hw_surface_lock(...);
*/



/* Dessine une ligne brisee avec l'algorithme d'approximation de Bresenham */
void	ei_draw_polyline	(ei_surface_t			surface,
						 const ei_linked_point_t*	first_point,
						 const ei_color_t		color,
						 const ei_rect_t*		clipper)
{
	if (first_point != NULL && first_point->next != NULL)
	{
		ei_point_t point_actuel;
		ei_point_t point_suivant;
		int delta_x;
		int delta_y;
		int erreur;

		while(first_point->next != NULL)
		{
			point_actuel = first_point->point;
			point_suivant = first_point->next->point;

			delta_x = point_suivant.x - point_actuel.x;
			delta_y = point_suivant.y - point_actuel.y;
			erreur = 0;

      /* Algo de Bresenham */

			if (delta_x)
			{
				if (delta_x > 0)
				{
					if (delta_y)
					{
						if (delta_y > 0)
						{
							if (delta_x >= delta_y)
							{
								change_pixel_value(surface, point_actuel, color);
								while (point_actuel.x != point_suivant.x)
								{
									point_actuel.x ++;
									erreur += delta_y;
									if (2 * erreur > delta_x)
									{
										point_actuel.y ++;
										erreur -= delta_x;
									}
									change_pixel_value(surface, point_actuel, color);
								}
							}
							if (delta_x < delta_y)
							{
								change_pixel_value(surface, point_actuel, color);
								while (point_actuel.y != point_suivant.y)
								{
									point_actuel.y ++;
									erreur += delta_x;
									if (2 * erreur > delta_y)
									{
										point_actuel.x ++;
										erreur -= delta_y;
									}
									change_pixel_value(surface, point_actuel, color);
								}
							}
						}
						else // delta_y < 0 et delta_x > 0
						{
							if (delta_x >= -delta_y)
							{
								change_pixel_value(surface, point_actuel, color);
								while (point_actuel.x != point_suivant.x)
								{
									point_actuel.x ++;
									erreur -= delta_y;
									if (2 * erreur > delta_x)
									{
										point_actuel.y --;
										erreur -= delta_x;
									}
									change_pixel_value(surface, point_actuel, color);
								}
							}
							if (delta_x < -delta_y)
							{
								change_pixel_value(surface, point_actuel, color);
								while (point_actuel.y != point_suivant.y)
								{
									point_actuel.y --;
									erreur += delta_x;
									if (2 * erreur > - delta_y)
									{
										point_actuel.x ++;
										erreur += delta_y;
									}
									change_pixel_value(surface, point_actuel, color);
								}
							}
						}
					}
					else // delta_y = 0 et delta_x > 0
					{
						change_pixel_value(surface, point_actuel, color);
						while (point_actuel.x != point_suivant.x)
						{
							point_actuel.x ++;
							change_pixel_value(surface, point_actuel, color);
						}
					}
				}
				else // delta_x < 0
				{
					if (delta_y)
					{
						if (delta_y > 0)
						{
							if (-delta_x >= delta_y)
							{
								change_pixel_value(surface, point_actuel, color);
								while (point_actuel.x != point_suivant.x)
								{
									point_actuel.x --;
									erreur += delta_y;
									if (2 * erreur > - delta_x)
									{
										point_actuel.y ++;
										erreur += delta_x;
									}
									change_pixel_value(surface, point_actuel, color);
								}
							}
							if (- delta_x < delta_y)
							{
								change_pixel_value(surface, point_actuel, color);
								while (point_actuel.y != point_suivant.y)
								{
									point_actuel.y ++;
									erreur -= delta_x;
									if (2 * erreur > delta_y)
									{
										point_actuel.x --;
										erreur -= delta_y;
									}
									change_pixel_value(surface, point_actuel, color);
								}
							}
						}
						else // delta_x < 0 et delta_y < 0
						{
							if (-delta_x >= -delta_y)
							{
								change_pixel_value(surface, point_actuel, color);
								while (point_actuel.x != point_suivant.x)
								{
									point_actuel.x --;
									erreur -= delta_y;
									if (2 * erreur > - delta_x)
									{
										point_actuel.y --;
										erreur += delta_x;
									}
									change_pixel_value(surface, point_actuel, color);
								}
							}
							if (-delta_x < -delta_y)
							{
								change_pixel_value(surface, point_actuel, color);
								while (point_actuel.y != point_suivant.y)
								{
									point_actuel.y --;
									erreur -= delta_x;
									if (2 * erreur > - delta_y)
									{
										point_actuel.x --;
										erreur += delta_y;
									}
									change_pixel_value(surface, point_actuel, color);
								}
							}
						}
					}
					else // delta_x < 0 et delta_y == 0
					{
						change_pixel_value(surface, point_actuel, color);
						while (point_actuel.x != point_suivant.x)
						{
							point_actuel.x --;
							change_pixel_value(surface, point_actuel, color);
						}
					}
				}
			}
			else // delta_x == 0
			{
				if (delta_y >= 0)
				{
					change_pixel_value(surface, point_actuel, color);
					while (point_actuel.y != point_suivant.y)
					{
						point_actuel.y ++;
						change_pixel_value(surface, point_actuel, color);
					}
				}
				if (delta_y < 0)
				{
					change_pixel_value(surface, point_actuel, color);
					while (point_actuel.y != point_suivant.y)
					{
						point_actuel.y --;
						change_pixel_value(surface, point_actuel, color);
					}
				}
			}
			first_point = first_point->next;
		}
	}
}


void	ei_fill(ei_surface_t surface,
						 const ei_color_t*	color,
						 const ei_rect_t*	clipper)
{
	ei_color_t fill_color = (color != NULL) ? *color : ei_default_background_color;
	uint32_t fill_color_int = ei_map_rgba(surface, &fill_color);

	ei_size_t taille_surface = hw_surface_get_size(surface);
	uint32_t* p_point_RAM = (uint32_t*) hw_surface_get_buffer(surface);

	for (int y = 0; y < taille_surface.width; y++)
	{
		for (int x = 0; x < taille_surface.height; x++)
		{
			ei_point_t current_point = {x,y};
			if (is_in_clipper(current_point, clipper))
			{
				*p_point_RAM = fill_color_int;
			}
			p_point_RAM ++;
		}
	}
}


uint32_t		ei_map_rgba		(ei_surface_t surface, const ei_color_t* color)
{
	int *ir = malloc(sizeof(int));
	int *ig = malloc(sizeof(int));
	int *ib = malloc(sizeof(int));
	int *ia = malloc(sizeof(int));

	hw_surface_get_channel_indices(surface, ir, ig, ib, ia);

	typedef union case_m_t{
		uint32_t result;
		uint8_t tab[4];
	} case_m_t;

	case_m_t couleur;
	couleur.tab[*ir] = color->red;
	couleur.tab[*ig] = color->green;
	couleur.tab[*ib] = color->blue;

	if (*ia != -1)
	{
		couleur.tab[*ia] = color->alpha;
	}

	free(ir);
	free(ig);
	free(ib);
	free(ia);

	return couleur.result;
}



int			ei_copy_surface		(ei_surface_t		destination,
						 const ei_rect_t*	dst_rect,
						 const ei_surface_t	source,
						 const ei_rect_t*	src_rect,
						 const ei_bool_t	alpha)
{
	ei_rect_t rect_a_copier = (src_rect != NULL) ? *src_rect : hw_surface_get_rect(source);
	ei_rect_t rect_ou_copier = (dst_rect != NULL) ? *dst_rect : hw_surface_get_rect(destination);

	if (rect_a_copier.size.width != rect_ou_copier.size.width || rect_a_copier.size.height != rect_ou_copier.size.height)
	{
		return 1;
	}

	int *ir_src = malloc(sizeof(int));
	int *ig_src = malloc(sizeof(int));
	int *ib_src = malloc(sizeof(int));
	int *ia_src = malloc(sizeof(int));

	int *ir_dest = malloc(sizeof(int));
	int *ig_dest = malloc(sizeof(int));
	int *ib_dest = malloc(sizeof(int));
	int *ia_dest = malloc(sizeof(int));


	ei_size_t taille_surface_dest = hw_surface_get_size(destination);
	ei_size_t taille_surface_src = hw_surface_get_size(source);

	uint32_t *p_point_origine_src = (uint32_t*) (hw_surface_get_buffer(source));
	uint32_t *p_point_origine_dest = (uint32_t*) (hw_surface_get_buffer(destination));

	hw_surface_get_channel_indices(source, ir_src, ig_src, ib_src, ia_src);
	hw_surface_get_channel_indices(source, ir_dest, ig_dest, ib_dest, ia_dest);

	for (int y = 0; y < rect_a_copier.size.height; y++)
	{
		for (int x = 0; x < rect_a_copier.size.width; x++)
		{
			uint32_t *p_point_act_src = p_point_origine_src + (rect_a_copier.top_left.y + y) * taille_surface_src.width + rect_a_copier.top_left.x + x;
			uint32_t *p_point_act_dest = p_point_origine_dest + (rect_ou_copier.top_left.y + y) * taille_surface_dest.width + rect_ou_copier.top_left.x + x;

			if (!alpha)
			{
				*p_point_act_dest = *p_point_act_src;
			}
			else
			{

				typedef union case_m_t{
					uint32_t result;
					uint8_t tab[4];
				} case_m_t;

				case_m_t couleur_src;
				couleur_src.result = *p_point_act_src;

				case_m_t couleur_dest;
				couleur_dest.result = *p_point_act_dest;

				couleur_dest.tab[*ir_dest] = (couleur_src.tab[*ia_src] * couleur_src.tab[*ir_src] + (255 - couleur_src.tab[*ia_src]) * couleur_dest.tab[*ir_dest]) / 255;
				couleur_dest.tab[*ig_dest] = (couleur_src.tab[*ia_src] * couleur_src.tab[*ig_src] + (255 - couleur_src.tab[*ia_src]) * couleur_dest.tab[*ig_dest]) / 255;
				couleur_dest.tab[*ib_dest] = (couleur_src.tab[*ia_src] * couleur_src.tab[*ib_src] + (255 - couleur_src.tab[*ia_src]) * couleur_dest.tab[*ib_dest]) / 255;
				couleur_dest.tab[*ia_dest] = 255;

				*p_point_act_dest = couleur_dest.result;
			}
		}
	}

	free(ir_src);
	free(ig_src);
	free(ib_src);
	free(ia_src);

	free(ir_dest);
	free(ig_dest);
	free(ib_dest);
	free(ia_dest);

	return 0;
}



void			ei_draw_text		(ei_surface_t		surface,
						 const ei_point_t*	where,
						 const char*		text,
						 const ei_font_t	font,
						 const ei_color_t*	color,
						 const ei_rect_t*	clipper)
{
	ei_surface_t surface_texte = hw_text_create_surface(text, font, color);
	ei_rect_t dest_rect_text;
	dest_rect_text.top_left = *where;
	dest_rect_text.size = hw_surface_get_size(surface_texte);

	hw_surface_lock(surface_texte);
	ei_copy_surface(surface, &dest_rect_text, surface_texte, clipper, EI_TRUE);
	hw_surface_unlock(surface_texte);
}

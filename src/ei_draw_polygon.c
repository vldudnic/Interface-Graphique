#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "../include/ei_draw.h"
#include "../include/aux_polygon.h"
#include "../include/ei_types.h"
#include "../include/aux_point.h"

void			ei_draw_polygon		(ei_surface_t			surface,
						 const ei_linked_point_t*	first_point,
						 const ei_color_t		color,
						 const ei_rect_t*		clipper)

{
  /* Calcul du y_min pout trouver l'indice de la 1ere scanline utile */
  int y_min = first_point->point.y, y_max = first_point->point.y;
  ei_linked_point_t *points_restants = first_point->next;
  ei_point_t point_suivant = points_restants->point;

  while (points_restants->next != NULL)
  {
    /* avec les deux points du segment considere, on met a jour y_min et y_max */
    y_min = (point_suivant.y < y_min) ? point_suivant.y : y_min;
    y_max = (point_suivant.y > y_max) ? point_suivant.y : y_max;

    /* on avance dans la liste de points du polygone */
    points_restants = points_restants->next;
    point_suivant = points_restants->point;
  }

	/* initialisation de TC avec les cotés du polygone */
	int taille_TC = y_max - y_min;
	ei_linked_cote_t **TC = malloc(taille_TC * sizeof(ei_linked_cote_t*));
	for (int i = 0; i < taille_TC; i++)
	{
		TC[i] = NULL;
	}

	ei_point_t point_actuel = first_point->point;
	points_restants = first_point->next;

	while (points_restants != NULL)
	{

		point_suivant = points_restants->point;

		int x_act = point_actuel.x, y_act = point_actuel.y;
    int x_suiv = point_suivant.x, y_suiv = point_suivant.y;

		ei_cote_t nouveau_cote;

		int delta_y = y_suiv - y_act;


		if (delta_y != 0)
		{
			nouveau_cote.delta_y = (delta_y < 0) ? -delta_y : delta_y;

			int delta_x = x_suiv - x_act;
			nouveau_cote.delta_x = (y_suiv > y_act) ? delta_x : -delta_x;

			nouveau_cote.y_max = (y_act > y_suiv) ? y_act : y_suiv;
			nouveau_cote.y_min = (y_act < y_suiv) ? y_act : y_suiv;
			nouveau_cote.x_y_min = (y_act < y_suiv) ? x_act : x_suiv;
			nouveau_cote.x_intersect = -1;
			nouveau_cote.erreur = 0;

			int scanline_debut = nouveau_cote.y_min - y_min;

			ei_linked_cote_t *first_cote = NULL;
			first_cote = malloc(sizeof(ei_linked_cote_t));
			first_cote->cote = nouveau_cote;
			first_cote->next = TC[scanline_debut];
			TC[scanline_debut] = first_cote;
		}

		points_restants = points_restants->next;
		point_actuel = point_suivant;
	}

	ei_linked_cote_t *TCA = NULL;
	for (int indice_scanline = 0; indice_scanline < taille_TC; indice_scanline++)
	{
		rajouter_cotes_TCA(&TCA, TC[indice_scanline]);
		supprimer_cotes_TCA(&TCA, indice_scanline + y_min);
		calcul_intersections(TCA, indice_scanline + y_min);
		tri_TCA(&TCA);
		colorier_scanline_TCA(surface, TCA, indice_scanline+y_min, color, clipper);
	}

	free_liste_cotes(TCA);
	for (int i = 0; i < taille_TC; i++)
	{
		free_liste_cotes(TC[i]);
	}

}

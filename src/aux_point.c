#include "../include/aux_point.h"
#include "../include/ei_draw.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>


void change_pixel_value(ei_surface_t surface, const ei_point_t point, const ei_color_t color)
{
  uint32_t *p_adresse_origine_surface = (uint32_t*) hw_surface_get_buffer(surface);
  ei_size_t taille_surface = hw_surface_get_size(surface);

	uint32_t *p_point_RAM = p_adresse_origine_surface + point.y * taille_surface.width + point.x;
  uint32_t couleur = ei_map_rgba(surface, &color);

	*p_point_RAM = couleur;
}


bool is_in_clipper(const ei_point_t point, const ei_rect_t *clipper)
{
  if (clipper == NULL)
  {
    return true;
  }

  int x_min = clipper->top_left.x, y_min = clipper->top_left.y;
  int x_max = x_min + clipper->size.width, y_max = clipper->size.height;
  int x = point.x, y = point.y;
  return (x_min <= x) && (x <= x_max) && (y_min <= y) && (y <= y_max);
}



void free_liste_points(ei_linked_point_t *liste_points)
{
  while (liste_points != NULL)
  {
    ei_linked_point_t *temp = liste_points;
    liste_points = liste_points->next;
    free(temp);
  }
  free(liste_points);
}



int len_liste_points(ei_linked_point_t *liste)
{
  int compteur = 0;
  while (liste != NULL)
  {
    liste = liste->next;
    compteur ++;
  }
  return compteur;
}


ei_linked_point_t* prendre_intervale_liste_points(ei_linked_point_t *liste, int debut, int fin)
{
  int indice = 0;
  ei_linked_point_t* liste_ret = NULL;
  ei_linked_point_t* curseur_liste_ret = liste_ret;
  while (indice <= fin)
  {
    if (indice >= debut)
    {
      ei_linked_point_t *nouveau_point = NULL;
      nouveau_point = malloc(sizeof(ei_linked_point_t));
      nouveau_point->point = liste->point;
      nouveau_point->next = NULL;
      if (liste_ret == NULL)
      {
        liste_ret = nouveau_point;
        curseur_liste_ret = nouveau_point;
      }
      else
      {
        curseur_liste_ret->next = nouveau_point;
        curseur_liste_ret = nouveau_point;
      }
    }
    liste = liste->next;
    indice ++;
  }
  return liste_ret;
}



void ajouter_point_solo_tete(ei_linked_point_t **p_liste, ei_point_t point)
{
  ei_linked_point_t *nouveau_point = malloc(sizeof(ei_linked_point_t));
  nouveau_point->point = point;
  nouveau_point->next = *p_liste;
  *p_liste = nouveau_point;
}



void inserer_liste_points_queue(ei_linked_point_t **p_liste, ei_linked_point_t *liste_ajoute)
{
  ei_linked_point_t *curseur = *p_liste;
  if (curseur != NULL)
  {
    while (curseur->next != NULL)
    {
      curseur = curseur->next;
    }
  }
  while (liste_ajoute != NULL)
  {
    ei_linked_point_t *nouveau_point = NULL;
    nouveau_point = malloc(sizeof(ei_linked_point_t));
    nouveau_point->point = liste_ajoute->point;
    nouveau_point->next = NULL;

    if (*p_liste == NULL)
    {
      *p_liste = nouveau_point;
      curseur = nouveau_point;
    }
    else
    {
      curseur->next = nouveau_point;
      curseur = curseur->next;
    }

    liste_ajoute = liste_ajoute->next;
  }
}

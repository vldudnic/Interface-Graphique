#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "../include/ei_draw.h"
#include "../include/aux_point.h"
#include "../include/aux_polygon.h"
#include "../include/ei_types.h"



void rajouter_cotes_TCA(ei_linked_cote_t **p_TCA, ei_linked_cote_t *cotes_rajouter)
{
  while (cotes_rajouter != NULL)
  {
    ei_linked_cote_t *nouveau_cote = malloc(sizeof(ei_linked_cote_t));
    nouveau_cote->cote = cotes_rajouter->cote;
    nouveau_cote->next = *p_TCA;
    *p_TCA = nouveau_cote;

    cotes_rajouter = cotes_rajouter->next;
  }
}


void supprimer_cotes_TCA(ei_linked_cote_t **p_TCA, int scanline)
{
  if (*p_TCA != NULL)
  {
    if ((*p_TCA)->cote.y_max == scanline)
    {
      ei_linked_cote_t *temp = *p_TCA;
      *p_TCA = (*p_TCA)->next;
      free(temp);
      supprimer_cotes_TCA(p_TCA, scanline);
    }
    else
    {
      supprimer_cotes_TCA(&((*p_TCA)->next), scanline);
    }
  }
}


void calcul_intersections(ei_linked_cote_t *first_cote, int scanline)
{
  while (first_cote != NULL)
  {
    int delta_x = first_cote->cote.delta_x;
    int delta_y = first_cote->cote.delta_y;

    if (scanline == first_cote->cote.y_min)
    {
      first_cote->cote.x_intersect = first_cote->cote.x_y_min;
    }
    else
    {
      if (delta_x)
      {
        if (delta_x > 0)
        {
          first_cote->cote.erreur += delta_x;
          while (2*first_cote->cote.erreur > delta_y)
          {
            first_cote->cote.x_intersect ++;
            first_cote->cote.erreur -= delta_y;
          }
        }
        else
        {
          first_cote->cote.erreur -= delta_x;
          while (2*first_cote->cote.erreur > delta_y)
          {
            first_cote->cote.x_intersect --;
            first_cote->cote.erreur -= delta_y;
          }
        }
      }
    }
    first_cote = first_cote->next;
  }
}



void tri_TCA(ei_linked_cote_t **p_TCA)
{
  ei_linked_cote_t *nouveau_TCA = NULL;
  while (*p_TCA != NULL)
  {
    ei_linked_cote_t *cote_max = NULL;
    cote_max = cote_max_TCA(*p_TCA);

    ei_linked_cote_t copie_cote_max;
    copie_cote_max.cote = cote_max->cote;
    copie_cote_max.next = NULL;

    rajouter_cotes_TCA(&nouveau_TCA, &copie_cote_max);
    supprime_cote_solo_TCA(p_TCA, cote_max);
  }
  rajouter_cotes_TCA(p_TCA, nouveau_TCA);
}



ei_linked_cote_t* cote_max_TCA(ei_linked_cote_t* TCA)
{
  ei_linked_cote_t* cote_max = TCA;
  while (TCA != NULL)
  {
    cote_max = (TCA->cote.x_intersect < cote_max->cote.x_intersect) ? TCA : cote_max;
    TCA = TCA->next;
  }
  return cote_max;
}



void supprime_cote_solo_TCA(ei_linked_cote_t **p_TCA, ei_linked_cote_t *cote_supprimer)
{
  if (*p_TCA != NULL)
  {
    if (*p_TCA == cote_supprimer)
    {
      ei_linked_cote_t * temp = NULL;
      temp = *p_TCA;
      *p_TCA = (*p_TCA)->next;
      free(temp);
    }
    else
    {
      ei_linked_cote_t *cotes = *p_TCA;
      ei_linked_cote_t *cotes_restants = (*p_TCA)->next;
      while (cotes_restants != cote_supprimer && cotes_restants != NULL)
      {
        cotes = cotes->next;
        cotes_restants = cotes_restants->next;
      }
      cotes->next = cotes_restants->next;
      free(cotes_restants);
    }
  }
}


void colorier_scanline_TCA(ei_surface_t surface, ei_linked_cote_t *TCA, int scanline, const ei_color_t color, const ei_rect_t *clipper)
{
  if (TCA != NULL)
  {
    bool colorier = true;
    while (TCA->next != NULL)
    {
      if (colorier)
      {
        ei_point_t point_colorier;
        point_colorier.y = scanline;
        for (int x = TCA->cote.x_intersect; x <= TCA->next->cote.x_intersect; x++)
        {
          point_colorier.x = x;
          if (is_in_clipper(point_colorier, clipper))
          {
            change_pixel_value(surface, point_colorier, color);
          }
        }
      }
      colorier = !colorier;
      TCA = TCA->next;
    }
  }
}



int len_liste_cotes(ei_linked_cote_t *liste)
{
  int compteur = 0;
  while (liste != NULL)
  {
    liste = liste->next;
    compteur ++;
  }
  return compteur;
}



void free_liste_cotes(ei_linked_cote_t *liste_cotes)
{
  if (liste_cotes != NULL)
  {
    if (liste_cotes->next == NULL)
    {
      free(liste_cotes);
    }
    else
    {
      free_liste_cotes(liste_cotes->next);
      free(liste_cotes);
    }
  }
}

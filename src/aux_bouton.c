#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../include/aux_point.h"
#include "../include/aux_bouton.h"
#include "../include/ei_draw.h"

 #define M_PI 3.1415


ei_linked_point_t* ei_draw_arc(ei_point_t origine, const int rayon, double debut, double fin)
{
  if (!rayon)
  {
    ei_linked_point_t *arc = malloc(sizeof(ei_linked_point_t));
    arc->point = origine;
    arc->next = NULL;
    return arc;
  }
  double pas = ((fin - debut))/(rayon);
  // l'arc sera decoupe en rayon points


  double angle = debut;
  //definition du premier point de l'arc
  int abscisse = floor(rayon*cos(angle));
  int ordonnee = floor(rayon*sin(angle));
  ei_point_t premier;
  premier.x = origine.x + abscisse;
  premier.y = origine.y - ordonnee;
  // creation de la liste de points de l'arc
  ei_linked_point_t* liste_arc = malloc(sizeof(ei_linked_point_t));
  liste_arc->point = premier;
  liste_arc->next = NULL;
  ei_linked_point_t * adresse_courante = liste_arc;

  while (angle < fin)
  {
    // on incremente l'angle
    angle += pas;
    // coordonnes du nouveau point qu'on va ajouter
    abscisse = floor(rayon*cosf(angle));
    ordonnee = floor(rayon*sinf(angle));
    //creation nouveau point de l'arc
    ei_point_t nouveau;
    nouveau.x = origine.x + abscisse;
    nouveau.y = origine.y - ordonnee;
    // ajout du nouveau point dans la liste des arcs

    ei_linked_point_t *nouveau_point = NULL;
    nouveau_point = malloc(sizeof(ei_linked_point_t));
    nouveau_point->point = nouveau;
    nouveau_point->next = NULL;
    adresse_courante->next = nouveau_point;
    adresse_courante = adresse_courante->next;
  }
  return liste_arc;
}



ei_linked_point_t*  rectangle_arrondi(const ei_rect_t rectangle, const int rayon)
{
  ei_linked_point_t* liste_finale = NULL;
  // calcul des quatre origines des quarts de cercle a tracer
  ei_point_t origine_1, origine_2, origine_3, origine_4;

  if (rectangle.size.width == 0 && rectangle.size.height == 0)
  {
    origine_1 = rectangle.top_left;
    origine_2 = rectangle.top_left;
    origine_3 = rectangle.top_left;
    origine_4 = rectangle.top_left;

  }
  else
  {
    //origine sup gauche
    origine_1.x = rectangle.top_left.x + rayon;
    origine_1.y = rectangle.top_left.y + rayon;

    //origine sup droit
    origine_2.x = rectangle.top_left.x + rectangle.size.width - rayon;
    origine_2.y = rectangle.top_left.y + rayon;

    //origine inf droit
    origine_3.x = rectangle.top_left.x + rectangle.size.width - rayon;
    origine_3.y = rectangle.top_left.y + rectangle.size.height - rayon;

    //origine inf gauche
    origine_4.x = rectangle.top_left.x + rayon;
    origine_4.y = rectangle.top_left.y + rectangle.size.height - rayon;
  }
  //construction des arcs

  ei_linked_point_t* tableau_arcs[5];


  tableau_arcs[0] = ei_draw_arc(origine_2, rayon, M_PI/4, M_PI/2);
  tableau_arcs[1] = ei_draw_arc(origine_1, rayon, M_PI/2, M_PI);
  tableau_arcs[2] = ei_draw_arc(origine_4, rayon, M_PI, 3*M_PI/2);
  tableau_arcs[3] = ei_draw_arc(origine_3, rayon, 3*M_PI/2, 2*M_PI);
  tableau_arcs[4] = ei_draw_arc(origine_2, rayon, 0, M_PI/4);

 for (size_t i = 0; i < 5; i++)
 {
   inserer_liste_points_queue(&liste_finale, tableau_arcs[i]);
 }

 for (int i = 0; i < 5; i++)
 {
   free_liste_points(tableau_arcs[i]);
 }

 return liste_finale;
}



void ei_draw_button_shape(ei_surface_t surface, const ei_rect_t rectangle, const int rayon, const int border_width, const ei_color_t color, ei_relief_t relief, ei_rect_t* clipper)
{

  ei_rect_t grand_rect;
  int grand_rayon;

  if (rectangle.size.width == 0 && rectangle.size.height == 0)
  {
    grand_rect = rectangle;
    grand_rayon = rayon + border_width;
  }
  else
  {
    grand_rect.top_left.x = rectangle.top_left.x - border_width;
    grand_rect.top_left.y = rectangle.top_left.y - border_width;
    grand_rect.size.width = rectangle.size.width + 2*border_width;
    grand_rect.size.height = rectangle.size.height + 2*border_width;
    grand_rayon = rayon;
  }

  ei_linked_point_t *liste_fond = rectangle_arrondi(grand_rect, grand_rayon);
  ei_linked_point_t *liste_top = rectangle_arrondi(rectangle, rayon);

  int taille_fond = len_liste_points(liste_fond);

  ei_linked_point_t *liste_haut = prendre_intervale_liste_points(liste_fond, 0, taille_fond/2);
  ei_linked_point_t *liste_bas = prendre_intervale_liste_points(liste_fond, taille_fond/2 , taille_fond-1);


  /* calcul des coordonnees des deux points centraux */
  int h = (rectangle.size.height)/2;
  ei_point_t centre_gauche, centre_droit;
  centre_gauche.x = rectangle.top_left.x + h;
  centre_gauche.y = rectangle.top_left.y + h;
  centre_droit.x = rectangle.top_left.x + rectangle.size.width - h;
  centre_droit.y = rectangle.top_left.y + h;

  /* ajout de ces deux points dans liste_haut et liste_bas */
  // fonction ajouter_en_tete a programmer

  ajouter_point_solo_tete(&liste_bas, centre_gauche);
  ajouter_point_solo_tete(&liste_bas, centre_droit);

  ajouter_point_solo_tete(&liste_haut, centre_droit);
  ajouter_point_solo_tete(&liste_haut, centre_gauche);

  ei_linked_point_t temp = {liste_haut->point, NULL};
  inserer_liste_points_queue(&liste_haut, &temp);

  temp.point = liste_bas->point;
  temp.next = NULL;
  inserer_liste_points_queue(&liste_bas, &temp);


  // definition couleur foncee , couleur claire
  ei_color_t color_foncee = {color.red / 2, color.green / 2, color.blue / 2, 0xff};
  ei_color_t color_claire = {(255 + color.red) / 2, (255 + color.green) / 2, (255 + color.blue) / 2, 0xff};


  if (relief == ei_relief_raised)
  {
    ei_draw_polygon(surface, liste_haut, color_claire, clipper);
    ei_draw_polygon(surface, liste_bas, color_foncee, clipper);
  }
  else if (relief == ei_relief_sunken)
  {
    ei_draw_polygon(surface, liste_haut, color_foncee, clipper);
    ei_draw_polygon(surface, liste_bas, color_claire, clipper);
  }
  else
  {
    ei_draw_polygon(surface, liste_haut, color, clipper);
    ei_draw_polygon(surface, liste_bas, color, clipper);
  }

  ei_draw_polygon(surface, liste_top, color, clipper);

  free_liste_points(liste_top);
  free_liste_points(liste_haut);
  free_liste_points(liste_bas);
}


ei_point_t where_to_place(ei_size_t object_size, ei_rect_t destination, ei_anchor_t anchor)
{
  ei_point_t ret_point;
  int object_full_width = object_size.width;
  int object_full_height = object_size.height;
  int object_half_width = object_size.width / 2;
  int object_half_height = object_size.height / 2;

  int dest_full_width = destination.size.width;
  int dest_full_height = destination.size.height;
  int dest_half_width = destination.size.width / 2;
  int dest_half_height = destination.size.height / 2;

  int x = destination.top_left.x;
  int y = destination.top_left.y;

  switch (anchor)
  {
    case ei_anc_center:
      ret_point.x = x + dest_half_width - object_half_width;
      ret_point.y = y + dest_half_height - object_half_height;
      break;
    default:
      break;
  }

  return ret_point;
}

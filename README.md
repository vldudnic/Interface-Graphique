Applications testées:

lines.c:
  test des differentes fonctions de ei_draw.h :
     - ei_fill, ei_fill, ei_copy_surface, ei_draw_text, ei_draw_polyline, ei_draw_polygon
  => tout semble fonctionner.

frame.c:
  test de l'affichage des differents widgets ainsi que de ei_app_create() ei_app_run():
    - frame, button, toplevel
  test aussi de l'affichage de widgets dans à l'interieur d'autres widgets;
    => affichage semble fonctionner
    remarque: nous n'avons pas arrondi les bordures des widgets top level.

button.c:
  test des events
  => les events bouton semblent fonctionner avec de la latence.
  => les events top level ne fonctionnent pas du tout.

Nous n'avons pas testé les autres applications.



Remarques générales:
  Beaucoup de fonctions ont étés implémentées dans leur version minimale non optimisée
  affin d'avoir un code fonctionnel et pouvoir tester d'autres fonctionalitées plus importantes

  Exemples:
    - ei_placer_run() ne prend pas en compte les cas où on essayerai de placer le widget en dehors de l'écran
      ou si on agrandi trop le widget
    - is_in_clipper() qui doit gerer le clipping n'est pas optimal il s'agit d'un simple test de coordonnées
      => clipping analytique à implémenter
    - l'ancrage des widgets du texte et des images à l'interieur des boutons/frames ne prend pour l'instant
      en compte que le cas par défault: ei_anc_center
    - ...

gimp_assert_vectors (GimpImage   *image,
                     const gchar *name,
                     GimpCoords   coords[],
                     gsize        coords_size,
                     gboolean     visible)
{
  GimpVectors *vectors        = NULL;
  GimpStroke  *stroke         = NULL;
  GArray      *control_points = NULL;
  gboolean     closed         = FALSE;
  gint         i              = 0;

  vectors = gimp_image_get_vectors_by_name (image, name);
  stroke = gimp_vectors_stroke_get_next (vectors, NULL);
  g_assert (stroke != NULL);
  control_points = gimp_stroke_control_points_get (stroke,
                                                   &closed);
  g_assert (closed);
  g_assert_cmpint (control_points->len,
                   ==,
                   coords_size);
  for (i = 0; i < control_points->len; i++)
    {
      g_assert_cmpint (coords[i].x,
                       ==,
                       g_array_index (control_points,
                                      GimpAnchor,
                                      i).position.x);
      g_assert_cmpint (coords[i].y,
                       ==,
                       g_array_index (control_points,
                                      GimpAnchor,
                                      i).position.y);
    }

  g_assert (gimp_item_get_visible (GIMP_ITEM (vectors)) ? TRUE : FALSE ==
            visible ? TRUE : FALSE);
}

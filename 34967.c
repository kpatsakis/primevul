pango_glyph_string_index_to_x (PangoGlyphString *glyphs,
			       char             *text,
			       int               length,
			       PangoAnalysis    *analysis,
			       int               index,
			       gboolean          trailing,
			       int              *x_pos)
{
  int i;
  int start_xpos = 0;
  int end_xpos = 0;
  int width = 0;

  int start_index = -1;
  int end_index = -1;

  int cluster_chars = 0;
  int cluster_offset = 0;

  char *p;

  g_return_if_fail (glyphs != NULL);
  g_return_if_fail (length >= 0);
  g_return_if_fail (length == 0 || text != NULL);

  if (!x_pos) /* Allow the user to do the useless */
    return;

  if (glyphs->num_glyphs == 0)
    {
      *x_pos = 0;
      return;
    }

  /* Calculate the starting and ending character positions
   * and x positions for the cluster
   */
  if (analysis->level % 2) /* Right to left */
    {
      for (i = glyphs->num_glyphs - 1; i >= 0; i--)
	width += glyphs->glyphs[i].geometry.width;

      for (i = glyphs->num_glyphs - 1; i >= 0; i--)
	{
	  if (glyphs->log_clusters[i] > index)
	    {
	      end_index = glyphs->log_clusters[i];
	      end_xpos = width;
	      break;
	    }

	  if (glyphs->log_clusters[i] != start_index)
	    {
	      start_index = glyphs->log_clusters[i];
	      start_xpos = width;
	    }

	  width -= glyphs->glyphs[i].geometry.width;
	}
    }
  else /* Left to right */
    {
      for (i = 0; i < glyphs->num_glyphs; i++)
	{
	  if (glyphs->log_clusters[i] > index)
	    {
	      end_index = glyphs->log_clusters[i];
	      end_xpos = width;
	      break;
	    }

	  if (glyphs->log_clusters[i] != start_index)
	    {
	      start_index = glyphs->log_clusters[i];
	      start_xpos = width;
	    }

	  width += glyphs->glyphs[i].geometry.width;
	}
    }

  if (end_index == -1)
    {
      end_index = length;
      end_xpos = (analysis->level % 2) ? 0 : width;
    }

  /* Calculate offset of character within cluster */

  p = text + start_index;
  while (p < text + end_index)
    {
      if (p < text + index)
	cluster_offset++;
      cluster_chars++;
      p = g_utf8_next_char (p);
    }

  if (trailing)
    cluster_offset += 1;

  *x_pos = ((cluster_chars - cluster_offset) * start_xpos +
	    cluster_offset * end_xpos) / cluster_chars;
}

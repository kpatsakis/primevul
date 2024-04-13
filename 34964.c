pango_glyph_string_get_logical_widths (PangoGlyphString *glyphs,
				       const char       *text,
				       int               length,
				       int               embedding_level,
				       int              *logical_widths)
{
  /* Build a PangoGlyphItem so we can use PangoGlyphItemIter.
   * This API should have been made to take a PangoGlyphItem... */
  PangoItem item = {0, length, g_utf8_strlen (text, length),
		    {NULL, NULL, NULL,
		     embedding_level, PANGO_GRAVITY_AUTO, 0,
		     PANGO_SCRIPT_UNKNOWN, NULL,
		     NULL}};
  PangoGlyphItem glyph_item = {&item, glyphs};
  PangoGlyphItemIter iter;
  gboolean has_cluster;
  int dir;

  dir = embedding_level % 2 == 0 ? +1 : -1;
  for (has_cluster = pango_glyph_item_iter_init_start (&iter, &glyph_item, text);
       has_cluster;
       has_cluster = pango_glyph_item_iter_next_cluster (&iter))
    {
      int glyph_index, char_index, num_chars, cluster_width = 0, char_width;

      for (glyph_index  = iter.start_glyph;
	   glyph_index != iter.end_glyph;
	   glyph_index += dir)
        {
	  cluster_width += glyphs->glyphs[glyph_index].geometry.width;
	}

      num_chars = iter.end_char - iter.start_char;
      if (num_chars) /* pedantic */
        {
	  char_width = cluster_width / num_chars;

	  for (char_index = iter.start_char;
	       char_index < iter.end_char;
	       char_index++)
	    {
	      logical_widths[char_index] = char_width;
	    }

	  /* add any residues to the first char */
	  logical_widths[iter.start_char] += cluster_width - (char_width * num_chars);
	}
    }
}

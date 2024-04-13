pango_glyph_string_copy (PangoGlyphString *string)
{
  PangoGlyphString *new_string;

  if (string == NULL)
    return NULL;
  
  new_string = g_slice_new (PangoGlyphString);

  *new_string = *string;

  new_string->glyphs = g_memdup (string->glyphs,
				 string->space * sizeof (PangoGlyphInfo));
  new_string->log_clusters = g_memdup (string->log_clusters,
				       string->space * sizeof (gint));

  return new_string;
}

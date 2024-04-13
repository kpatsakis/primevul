pango_glyph_string_new (void)
{
  PangoGlyphString *string = g_slice_new (PangoGlyphString);

  string->num_glyphs = 0;
  string->space = 0;
  string->glyphs = NULL;
  string->log_clusters = NULL;

  return string;
}

pango_glyph_string_get_type (void)
{
  static GType our_type = 0;

  if (G_UNLIKELY (our_type == 0))
    our_type = g_boxed_type_register_static (I_("PangoGlyphString"),
					     (GBoxedCopyFunc)pango_glyph_string_copy,
					     (GBoxedFreeFunc)pango_glyph_string_free);

  return our_type;
}

pango_glyph_string_extents (PangoGlyphString *glyphs,
			    PangoFont        *font,
			    PangoRectangle   *ink_rect,
			    PangoRectangle   *logical_rect)
{
  pango_glyph_string_extents_range (glyphs, 0, glyphs->num_glyphs,
				    font, ink_rect, logical_rect);
}

  ps_hints_apply( PS_Hints        ps_hints,
                  FT_Outline*     outline,
                  PSH_Globals     globals,
                  FT_Render_Mode  hint_mode )
  {
    PSH_GlyphRec  glyphrec;
    PSH_Glyph     glyph = &glyphrec;
    FT_Error      error;
#ifdef DEBUG_HINTER
    FT_Memory     memory;
#endif
    FT_Int        dimension;


    /* something to do? */
    if ( outline->n_points == 0 || outline->n_contours == 0 )
      return PSH_Err_Ok;

#ifdef DEBUG_HINTER

    memory = globals->memory;

    if ( ps_debug_glyph )
    {
      psh_glyph_done( ps_debug_glyph );
      FT_FREE( ps_debug_glyph );
    }

    if ( FT_NEW( glyph ) )
      return error;

    ps_debug_glyph = glyph;

#endif /* DEBUG_HINTER */

    error = psh_glyph_init( glyph, outline, ps_hints, globals );
    if ( error )
      goto Exit;

    /* try to optimize the y_scale so that the top of non-capital letters
     * is aligned on a pixel boundary whenever possible
     */
    {
      PSH_Dimension  dim_x = &glyph->globals->dimension[0];
      PSH_Dimension  dim_y = &glyph->globals->dimension[1];

      FT_Fixed  x_scale = dim_x->scale_mult;
      FT_Fixed  y_scale = dim_y->scale_mult;

      FT_Fixed  old_x_scale = x_scale;
      FT_Fixed  old_y_scale = y_scale;

      FT_Fixed  scaled;
      FT_Fixed  fitted;

      FT_Bool  rescale = FALSE;


      scaled = FT_MulFix( globals->blues.normal_top.zones->org_ref, y_scale );
      fitted = FT_PIX_ROUND( scaled );

      if ( fitted != 0 && scaled != fitted )
      {
        rescale = TRUE;

        y_scale = FT_MulDiv( y_scale, fitted, scaled );

        if ( fitted < scaled )
          x_scale -= x_scale / 50;

        psh_globals_set_scale( glyph->globals, x_scale, y_scale, 0, 0 );
      }

      glyph->do_horz_hints = 1;
      glyph->do_vert_hints = 1;

      glyph->do_horz_snapping = FT_BOOL( hint_mode == FT_RENDER_MODE_MONO ||
                                         hint_mode == FT_RENDER_MODE_LCD  );

      glyph->do_vert_snapping = FT_BOOL( hint_mode == FT_RENDER_MODE_MONO  ||
                                         hint_mode == FT_RENDER_MODE_LCD_V );

      glyph->do_stem_adjust   = FT_BOOL( hint_mode != FT_RENDER_MODE_LIGHT );

      for ( dimension = 0; dimension < 2; dimension++ )
      {
        /* load outline coordinates into glyph */
        psh_glyph_load_points( glyph, dimension );

        /* compute local extrema */
        psh_glyph_compute_extrema( glyph );

        /* compute aligned stem/hints positions */
        psh_hint_table_align_hints( &glyph->hint_tables[dimension],
                                    glyph->globals,
                                    dimension,
                                    glyph );

        /* find strong points, align them, then interpolate others */
        psh_glyph_find_strong_points( glyph, dimension );
        if ( dimension == 1 )
          psh_glyph_find_blue_points( &globals->blues, glyph );
        psh_glyph_interpolate_strong_points( glyph, dimension );
        psh_glyph_interpolate_normal_points( glyph, dimension );
        psh_glyph_interpolate_other_points( glyph, dimension );

        /* save hinted coordinates back to outline */
        psh_glyph_save_points( glyph, dimension );

        if ( rescale )
          psh_globals_set_scale( glyph->globals,
                                 old_x_scale, old_y_scale, 0, 0 );
      }
    }

  Exit:

#ifndef DEBUG_HINTER
    psh_glyph_done( glyph );
#endif

    return error;
  }

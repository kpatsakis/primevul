  cff_slot_load( CFF_GlyphSlot  glyph,
                 CFF_Size       size,
                 FT_UInt        glyph_index,
                 FT_Int32       load_flags )
  {
    FT_Error     error;
    CFF_Decoder  decoder;
    TT_Face      face = (TT_Face)glyph->root.face;
    FT_Bool      hinting, force_scaling;
    CFF_Font     cff  = (CFF_Font)face->extra.data;

    FT_Matrix    font_matrix;
    FT_Vector    font_offset;


    force_scaling = FALSE;

    /* in a CID-keyed font, consider `glyph_index' as a CID and map */
    /* it immediately to the real glyph_index -- if it isn't a      */
    /* subsetted font, glyph_indices and CIDs are identical, though */
    if ( cff->top_font.font_dict.cid_registry != 0xFFFFU &&
         cff->charset.cids                               )
    {
      /* don't handle CID 0 (.notdef) which is directly mapped to GID 0 */
      if ( glyph_index != 0 )
      {
        glyph_index = cff_charset_cid_to_gindex( &cff->charset,
                                                 glyph_index );
        if ( glyph_index == 0 )
          return CFF_Err_Invalid_Argument;
      }
    }
    else if ( glyph_index >= cff->num_glyphs )
      return CFF_Err_Invalid_Argument;

    if ( load_flags & FT_LOAD_NO_RECURSE )
      load_flags |= FT_LOAD_NO_SCALE | FT_LOAD_NO_HINTING;

    glyph->x_scale = 0x10000L;
    glyph->y_scale = 0x10000L;
    if ( size )
    {
      glyph->x_scale = size->root.metrics.x_scale;
      glyph->y_scale = size->root.metrics.y_scale;
    }

#ifdef TT_CONFIG_OPTION_EMBEDDED_BITMAPS

    /* try to load embedded bitmap if any              */
    /*                                                 */
    /* XXX: The convention should be emphasized in     */
    /*      the documents because it can be confusing. */
    if ( size )
    {
      CFF_Face      cff_face = (CFF_Face)size->root.face;
      SFNT_Service  sfnt     = (SFNT_Service)cff_face->sfnt;
      FT_Stream     stream   = cff_face->root.stream;


      if ( size->strike_index != 0xFFFFFFFFUL      &&
           sfnt->load_eblc                         &&
           ( load_flags & FT_LOAD_NO_BITMAP ) == 0 )
      {
        TT_SBit_MetricsRec  metrics;


        error = sfnt->load_sbit_image( face,
                                       size->strike_index,
                                       glyph_index,
                                       (FT_Int)load_flags,
                                       stream,
                                       &glyph->root.bitmap,
                                       &metrics );

        if ( !error )
        {
          glyph->root.outline.n_points   = 0;
          glyph->root.outline.n_contours = 0;

          glyph->root.metrics.width  = (FT_Pos)metrics.width  << 6;
          glyph->root.metrics.height = (FT_Pos)metrics.height << 6;

          glyph->root.metrics.horiBearingX = (FT_Pos)metrics.horiBearingX << 6;
          glyph->root.metrics.horiBearingY = (FT_Pos)metrics.horiBearingY << 6;
          glyph->root.metrics.horiAdvance  = (FT_Pos)metrics.horiAdvance  << 6;

          glyph->root.metrics.vertBearingX = (FT_Pos)metrics.vertBearingX << 6;
          glyph->root.metrics.vertBearingY = (FT_Pos)metrics.vertBearingY << 6;
          glyph->root.metrics.vertAdvance  = (FT_Pos)metrics.vertAdvance  << 6;

          glyph->root.format = FT_GLYPH_FORMAT_BITMAP;

          if ( load_flags & FT_LOAD_VERTICAL_LAYOUT )
          {
            glyph->root.bitmap_left = metrics.vertBearingX;
            glyph->root.bitmap_top  = metrics.vertBearingY;
          }
          else
          {
            glyph->root.bitmap_left = metrics.horiBearingX;
            glyph->root.bitmap_top  = metrics.horiBearingY;
          }
          return error;
        }
      }
    }

#endif /* TT_CONFIG_OPTION_EMBEDDED_BITMAPS */

    /* return immediately if we only want the embedded bitmaps */
    if ( load_flags & FT_LOAD_SBITS_ONLY )
      return CFF_Err_Invalid_Argument;

    /* if we have a CID subfont, use its matrix (which has already */
    /* been multiplied with the root matrix)                       */

    /* this scaling is only relevant if the PS hinter isn't active */
    if ( cff->num_subfonts )
    {
      FT_ULong  top_upm, sub_upm;
      FT_Byte   fd_index = cff_fd_select_get( &cff->fd_select,
                                              glyph_index );

      if ( fd_index >= cff->num_subfonts ) 
        fd_index = cff->num_subfonts - 1;

      top_upm = cff->top_font.font_dict.units_per_em;
      sub_upm = cff->subfonts[fd_index]->font_dict.units_per_em;


      font_matrix = cff->subfonts[fd_index]->font_dict.font_matrix;
      font_offset = cff->subfonts[fd_index]->font_dict.font_offset;

      if ( top_upm != sub_upm )
      {
        glyph->x_scale = FT_MulDiv( glyph->x_scale, top_upm, sub_upm );
        glyph->y_scale = FT_MulDiv( glyph->y_scale, top_upm, sub_upm );

        force_scaling = TRUE;
      }
    }
    else
    {
      font_matrix = cff->top_font.font_dict.font_matrix;
      font_offset = cff->top_font.font_dict.font_offset;
    }

    glyph->root.outline.n_points   = 0;
    glyph->root.outline.n_contours = 0;

    hinting = FT_BOOL( ( load_flags & FT_LOAD_NO_SCALE   ) == 0 &&
                       ( load_flags & FT_LOAD_NO_HINTING ) == 0 );

    glyph->root.format = FT_GLYPH_FORMAT_OUTLINE;  /* by default */

    {
      FT_Byte*  charstring;
      FT_ULong  charstring_len;


      cff_decoder_init( &decoder, face, size, glyph, hinting,
                        FT_LOAD_TARGET_MODE( load_flags ) );

      if ( load_flags & FT_LOAD_ADVANCE_ONLY )
        decoder.width_only = TRUE;

      decoder.builder.no_recurse =
        (FT_Bool)( load_flags & FT_LOAD_NO_RECURSE );

      /* now load the unscaled outline */
      error = cff_get_glyph_data( face, glyph_index,
                                  &charstring, &charstring_len );
      if ( !error )
      {
        error = cff_decoder_prepare( &decoder, size, glyph_index );
        if ( !error )
        {
          error = cff_decoder_parse_charstrings( &decoder,
                                                 charstring,
                                                 charstring_len );

          cff_free_glyph_data( face, &charstring, charstring_len );


#ifdef FT_CONFIG_OPTION_INCREMENTAL
          /* Control data and length may not be available for incremental */
          /* fonts.                                                       */
          if ( face->root.internal->incremental_interface )
          {
            glyph->root.control_data = 0;
            glyph->root.control_len = 0;
          }
          else
#endif /* FT_CONFIG_OPTION_INCREMENTAL */

          /* We set control_data and control_len if charstrings is loaded. */
          /* See how charstring loads at cff_index_access_element() in     */
          /* cffload.c.                                                    */
          {
            CFF_Index  csindex = &cff->charstrings_index;


            if ( csindex->offsets )
            {
              glyph->root.control_data = csindex->bytes +
                                           csindex->offsets[glyph_index] - 1;
              glyph->root.control_len  = charstring_len;
            }
          }
        }
      }

      /* save new glyph tables */
      cff_builder_done( &decoder.builder );
    }

#ifdef FT_CONFIG_OPTION_INCREMENTAL

    /* Incremental fonts can optionally override the metrics. */
    if ( !error                                                               &&
         face->root.internal->incremental_interface                           &&
         face->root.internal->incremental_interface->funcs->get_glyph_metrics )
    {
      FT_Incremental_MetricsRec  metrics;


      metrics.bearing_x = decoder.builder.left_bearing.x;
      metrics.bearing_y = 0;
      metrics.advance   = decoder.builder.advance.x;
      metrics.advance_v = decoder.builder.advance.y;

      error = face->root.internal->incremental_interface->funcs->get_glyph_metrics(
                face->root.internal->incremental_interface->object,
                glyph_index, FALSE, &metrics );

      decoder.builder.left_bearing.x = metrics.bearing_x;
      decoder.builder.advance.x      = metrics.advance;
      decoder.builder.advance.y      = metrics.advance_v;
    }

#endif /* FT_CONFIG_OPTION_INCREMENTAL */

    if ( !error )
    {
      /* Now, set the metrics -- this is rather simple, as   */
      /* the left side bearing is the xMin, and the top side */
      /* bearing the yMax.                                   */

      /* For composite glyphs, return only left side bearing and */
      /* advance width.                                          */
      if ( load_flags & FT_LOAD_NO_RECURSE )
      {
        FT_Slot_Internal  internal = glyph->root.internal;


        glyph->root.metrics.horiBearingX = decoder.builder.left_bearing.x;
        glyph->root.metrics.horiAdvance  = decoder.glyph_width;
        internal->glyph_matrix           = font_matrix;
        internal->glyph_delta            = font_offset;
        internal->glyph_transformed      = 1;
      }
      else
      {
        FT_BBox            cbox;
        FT_Glyph_Metrics*  metrics = &glyph->root.metrics;
        FT_Vector          advance;
        FT_Bool            has_vertical_info;


        /* copy the _unscaled_ advance width */
        metrics->horiAdvance                    = decoder.glyph_width;
        glyph->root.linearHoriAdvance           = decoder.glyph_width;
        glyph->root.internal->glyph_transformed = 0;

#ifdef FT_CONFIG_OPTION_OLD_INTERNALS
        has_vertical_info = FT_BOOL( face->vertical_info                   &&
                                     face->vertical.number_Of_VMetrics > 0 &&
                                     face->vertical.long_metrics           );
#else
        has_vertical_info = FT_BOOL( face->vertical_info                   &&
                                     face->vertical.number_Of_VMetrics > 0 );
#endif

        /* get the vertical metrics from the vtmx table if we have one */
        if ( has_vertical_info )
        {
          FT_Short   vertBearingY = 0;
          FT_UShort  vertAdvance  = 0;


          ( (SFNT_Service)face->sfnt )->get_metrics( face, 1,
                                                     glyph_index,
                                                     &vertBearingY,
                                                     &vertAdvance );
          metrics->vertBearingY = vertBearingY;
          metrics->vertAdvance  = vertAdvance;
        }
        else
        {
          /* make up vertical ones */
          if ( face->os2.version != 0xFFFFU )
            metrics->vertAdvance = (FT_Pos)( face->os2.sTypoAscender -
                                             face->os2.sTypoDescender );
          else
            metrics->vertAdvance = (FT_Pos)( face->horizontal.Ascender -
                                             face->horizontal.Descender );
        }

        glyph->root.linearVertAdvance = metrics->vertAdvance;

        glyph->root.format = FT_GLYPH_FORMAT_OUTLINE;

        glyph->root.outline.flags = 0;
        if ( size && size->root.metrics.y_ppem < 24 )
          glyph->root.outline.flags |= FT_OUTLINE_HIGH_PRECISION;

        glyph->root.outline.flags |= FT_OUTLINE_REVERSE_FILL;

        if ( !( font_matrix.xx == 0x10000L &&
                font_matrix.yy == 0x10000L &&
                font_matrix.xy == 0        &&
                font_matrix.yx == 0        ) )
          FT_Outline_Transform( &glyph->root.outline, &font_matrix );

        if ( !( font_offset.x == 0 &&
                font_offset.y == 0 ) )
          FT_Outline_Translate( &glyph->root.outline,
                                font_offset.x, font_offset.y );

        advance.x = metrics->horiAdvance;
        advance.y = 0;
        FT_Vector_Transform( &advance, &font_matrix );
        metrics->horiAdvance = advance.x + font_offset.x;

        advance.x = 0;
        advance.y = metrics->vertAdvance;
        FT_Vector_Transform( &advance, &font_matrix );
        metrics->vertAdvance = advance.y + font_offset.y;

        if ( ( load_flags & FT_LOAD_NO_SCALE ) == 0 || force_scaling )
        {
          /* scale the outline and the metrics */
          FT_Int       n;
          FT_Outline*  cur     = &glyph->root.outline;
          FT_Vector*   vec     = cur->points;
          FT_Fixed     x_scale = glyph->x_scale;
          FT_Fixed     y_scale = glyph->y_scale;


          /* First of all, scale the points */
          if ( !hinting || !decoder.builder.hints_funcs )
            for ( n = cur->n_points; n > 0; n--, vec++ )
            {
              vec->x = FT_MulFix( vec->x, x_scale );
              vec->y = FT_MulFix( vec->y, y_scale );
            }

          /* Then scale the metrics */
          metrics->horiAdvance = FT_MulFix( metrics->horiAdvance, x_scale );
          metrics->vertAdvance = FT_MulFix( metrics->vertAdvance, y_scale );
        }

        /* compute the other metrics */
        FT_Outline_Get_CBox( &glyph->root.outline, &cbox );

        metrics->width  = cbox.xMax - cbox.xMin;
        metrics->height = cbox.yMax - cbox.yMin;

        metrics->horiBearingX = cbox.xMin;
        metrics->horiBearingY = cbox.yMax;

        if ( has_vertical_info )
          metrics->vertBearingX = metrics->horiBearingX -
                                    metrics->horiAdvance / 2;
        else 
        {
          if ( load_flags & FT_LOAD_VERTICAL_LAYOUT )
            ft_synthesize_vertical_metrics( metrics,
                                            metrics->vertAdvance );
        }
      }
    }

    return error;
  }

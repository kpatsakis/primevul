  cff_decoder_prepare( CFF_Decoder*  decoder,
                       CFF_Size      size,
                       FT_UInt       glyph_index )
  {
    CFF_Builder  *builder = &decoder->builder;
    CFF_Font      cff     = (CFF_Font)builder->face->extra.data;
    CFF_SubFont   sub     = &cff->top_font;
    FT_Error      error   = CFF_Err_Ok;


    /* manage CID fonts */
    if ( cff->num_subfonts )
    {
      FT_Byte  fd_index = cff_fd_select_get( &cff->fd_select, glyph_index );


      if ( fd_index >= cff->num_subfonts )
      {
        FT_TRACE4(( "cff_decoder_prepare: invalid CID subfont index\n" ));
        error = CFF_Err_Invalid_File_Format;
        goto Exit;
      }

      FT_TRACE4(( "glyph index %d (subfont %d):\n", glyph_index, fd_index ));

      sub = cff->subfonts[fd_index];

      if ( builder->hints_funcs && size )
      {
        CFF_Internal  internal = (CFF_Internal)size->root.internal;


        /* for CFFs without subfonts, this value has already been set */
        builder->hints_globals = (void *)internal->subfonts[fd_index];
      }
    }
#ifdef FT_DEBUG_LEVEL_TRACE
    else
      FT_TRACE4(( "glyph index %d:\n", glyph_index ));
#endif

    decoder->num_locals    = sub->local_subrs_index.count;
    decoder->locals        = sub->local_subrs;
    decoder->locals_bias   = cff_compute_bias(
                               decoder->cff->top_font.font_dict.charstring_type,
                               decoder->num_locals );

    decoder->glyph_width   = sub->private_dict.default_width;
    decoder->nominal_width = sub->private_dict.nominal_width;

  Exit:
    return error;
  }

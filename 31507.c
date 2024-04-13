  bdf_load_font( FT_Stream       stream,
                 FT_Memory       extmemory,
                 bdf_options_t*  opts,
                 bdf_font_t*    *font )
  {
    unsigned long  lineno = 0; /* make compiler happy */
    _bdf_parse_t   *p     = NULL;

    FT_Memory      memory = extmemory;
    FT_Error       error  = BDF_Err_Ok;


    if ( FT_NEW( p ) )
      goto Exit;

    memory    = NULL;
    p->opts   = (bdf_options_t*)( ( opts != 0 ) ? opts : &_bdf_opts );
    p->minlb  = 32767;
    p->memory = extmemory;  /* only during font creation */

    _bdf_list_init( &p->list, extmemory );

    error = _bdf_readstream( stream, _bdf_parse_start,
                             (void *)p, &lineno );
    if ( error )
      goto Fail;

    if ( p->font != 0 )
    {
      /* If the font is not proportional, set the font's monowidth */
      /* field to the width of the font bounding box.              */
      memory = p->font->memory;

      if ( p->font->spacing != BDF_PROPORTIONAL )
        p->font->monowidth = p->font->bbx.width;

      /* If the number of glyphs loaded is not that of the original count, */
      /* indicate the difference.                                          */
      if ( p->cnt != p->font->glyphs_used + p->font->unencoded_used )
      {
        FT_TRACE2(( "bdf_load_font: " ACMSG15, p->cnt,
                    p->font->glyphs_used + p->font->unencoded_used ));
        p->font->modified = 1;
      }

      /* Once the font has been loaded, adjust the overall font metrics if */
      /* necessary.                                                        */
      if ( p->opts->correct_metrics != 0 &&
           ( p->font->glyphs_used > 0 || p->font->unencoded_used > 0 ) )
      {
        if ( p->maxrb - p->minlb != p->font->bbx.width )
        {
          FT_TRACE2(( "bdf_load_font: " ACMSG3,
                      p->font->bbx.width, p->maxrb - p->minlb ));
          p->font->bbx.width = (unsigned short)( p->maxrb - p->minlb );
          p->font->modified  = 1;
        }

        if ( p->font->bbx.x_offset != p->minlb )
        {
          FT_TRACE2(( "bdf_load_font: " ACMSG4,
                      p->font->bbx.x_offset, p->minlb ));
          p->font->bbx.x_offset = p->minlb;
          p->font->modified     = 1;
        }

        if ( p->font->bbx.ascent != p->maxas )
        {
          FT_TRACE2(( "bdf_load_font: " ACMSG5,
                      p->font->bbx.ascent, p->maxas ));
          p->font->bbx.ascent = p->maxas;
          p->font->modified   = 1;
        }

        if ( p->font->bbx.descent != p->maxds )
        {
          FT_TRACE2(( "bdf_load_font: " ACMSG6,
                      p->font->bbx.descent, p->maxds ));
          p->font->bbx.descent  = p->maxds;
          p->font->bbx.y_offset = (short)( -p->maxds );
          p->font->modified     = 1;
        }

        if ( p->maxas + p->maxds != p->font->bbx.height )
        {
          FT_TRACE2(( "bdf_load_font: " ACMSG7,
                      p->font->bbx.height, p->maxas + p->maxds ));
          p->font->bbx.height = (unsigned short)( p->maxas + p->maxds );
        }

        if ( p->flags & _BDF_SWIDTH_ADJ )
          FT_TRACE2(( "bdf_load_font: " ACMSG8 ));
      }
    }

    if ( p->flags & _BDF_START )
    {
      /* The ENDFONT field was never reached or did not exist. */
      if ( !( p->flags & _BDF_GLYPHS ) )
      {
        /* Error happened while parsing header. */
        FT_ERROR(( "bdf_load_font: " ERRMSG2, lineno ));
        error = BDF_Err_Corrupted_Font_Header;
        goto Exit;
      }
      else
      {
        /* Error happened when parsing glyphs. */
        FT_ERROR(( "bdf_load_font: " ERRMSG3, lineno ));
        error = BDF_Err_Corrupted_Font_Glyphs;
        goto Exit;
      }
    }

    if ( p->font != 0 )
    {
      /* Make sure the comments are NULL terminated if they exist. */
      memory = p->font->memory;

      if ( p->font->comments_len > 0 )
      {
        if ( FT_RENEW_ARRAY( p->font->comments,
                             p->font->comments_len,
                             p->font->comments_len + 1 ) )
          goto Fail;

        p->font->comments[p->font->comments_len] = 0;
      }
    }
    else if ( error == BDF_Err_Ok )
      error = BDF_Err_Invalid_File_Format;

    *font = p->font;

  Exit:
    if ( p )
    {
      _bdf_list_done( &p->list );

      memory = extmemory;

      FT_FREE( p );
    }

    return error;

  Fail:
    bdf_free_font( p->font );

    memory = extmemory;

    FT_FREE( p->font );

    goto Exit;
  }

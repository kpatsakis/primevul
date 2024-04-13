  tt_cmap12_next( TT_CMap12  cmap )
  {
    FT_Face   face = cmap->cmap.cmap.charmap.face;
    FT_Byte*  p;
    FT_ULong  start, end, start_id, char_code;
    FT_ULong  n;
    FT_UInt   gindex;


    if ( cmap->cur_charcode >= 0xFFFFFFFFUL )
      goto Fail;

    char_code = cmap->cur_charcode + 1;

    for ( n = cmap->cur_group; n < cmap->num_groups; n++ )
    {
      p        = cmap->cmap.data + 16 + 12 * n;
      start    = TT_NEXT_ULONG( p );
      end      = TT_NEXT_ULONG( p );
      start_id = TT_PEEK_ULONG( p );

      if ( char_code < start )
        char_code = start;

    Again:
      if ( char_code <= end )
      {
        /* ignore invalid group */
        if ( start_id > 0xFFFFFFFFUL - ( char_code - start ) )
          continue;

        gindex = (FT_UInt)( start_id + ( char_code - start ) );

        /* does first element of group point to `.notdef' glyph? */
        if ( gindex == 0 )
        {
          if ( char_code >= 0xFFFFFFFFUL )
            goto Fail;

          char_code++;
          goto Again;
        }

        /* if `gindex' is invalid, the remaining values */
        /* in this group are invalid, too               */
        if ( gindex >= (FT_UInt)face->num_glyphs )
        {
          gindex = 0;
          continue;
        }

        cmap->cur_charcode = char_code;
        cmap->cur_gindex   = gindex;
        cmap->cur_group    = n;

        return;
      }
    }

  Fail:
    cmap->valid = 0;
  }

  tt_cmap8_char_next( TT_CMap     cmap,
                      FT_UInt32  *pchar_code )
  {
    FT_Face    face       = cmap->cmap.charmap.face;
    FT_UInt32  result     = 0;
    FT_UInt32  char_code;
    FT_UInt    gindex     = 0;
    FT_Byte*   table      = cmap->data;
    FT_Byte*   p          = table + 8204;
    FT_UInt32  num_groups = TT_NEXT_ULONG( p );
    FT_UInt32  start, end, start_id;


    if ( *pchar_code >= 0xFFFFFFFFUL )
      return 0;

    char_code = *pchar_code + 1;

    p = table + 8208;

    for ( ; num_groups > 0; num_groups-- )
    {
      start    = TT_NEXT_ULONG( p );
      end      = TT_NEXT_ULONG( p );
      start_id = TT_NEXT_ULONG( p );

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
            break;

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

        result = char_code;
        break;
      }
    }

    *pchar_code = result;
    return gindex;
  }

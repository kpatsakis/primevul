  tt_cmap4_char_map_linear( TT_CMap     cmap,
                            FT_UInt32*  pcharcode,
                            FT_Bool     next )
  {
    TT_Face   face  = (TT_Face)cmap->cmap.charmap.face;
    FT_Byte*  limit = face->cmap_table + face->cmap_size;


    FT_UInt    num_segs2, start, end, offset;
    FT_Int     delta;
    FT_UInt    i, num_segs;
    FT_UInt32  charcode = *pcharcode;
    FT_UInt    gindex   = 0;
    FT_Byte*   p;
    FT_Byte*   q;


    p = cmap->data + 6;
    num_segs2 = FT_PAD_FLOOR( TT_PEEK_USHORT( p ), 2 );

    num_segs = num_segs2 >> 1;

    if ( !num_segs )
      return 0;

    if ( next )
      charcode++;

    if ( charcode > 0xFFFFU )
      return 0;

    /* linear search */
    p = cmap->data + 14;               /* ends table   */
    q = cmap->data + 16 + num_segs2;   /* starts table */

    for ( i = 0; i < num_segs; i++ )
    {
      end   = TT_NEXT_USHORT( p );
      start = TT_NEXT_USHORT( q );

      if ( charcode < start )
      {
        if ( next )
          charcode = start;
        else
          break;
      }

    Again:
      if ( charcode <= end )
      {
        FT_Byte*  r;


        r       = q - 2 + num_segs2;
        delta   = TT_PEEK_SHORT( r );
        r      += num_segs2;
        offset  = TT_PEEK_USHORT( r );

        /* some fonts have an incorrect last segment; */
        /* we have to catch it                        */
        if ( i >= num_segs - 1                  &&
             start == 0xFFFFU && end == 0xFFFFU )
        {
          if ( offset && r + offset + 2 > limit )
          {
            delta  = 1;
            offset = 0;
          }
        }

        if ( offset == 0xFFFFU )
          continue;

        if ( offset )
        {
          r += offset + ( charcode - start ) * 2;

          /* if r > limit, the whole segment is invalid */
          if ( next && r > limit )
            continue;

          gindex = TT_PEEK_USHORT( r );
          if ( gindex )
          {
            gindex = (FT_UInt)( (FT_Int)gindex + delta ) & 0xFFFFU;
            if ( gindex >= (FT_UInt)face->root.num_glyphs )
              gindex = 0;
          }
        }
        else
        {
          gindex = (FT_UInt)( (FT_Int)charcode + delta ) & 0xFFFFU;

          if ( next && gindex >= (FT_UInt)face->root.num_glyphs )
          {
            /* we have an invalid glyph index; if there is an overflow, */
            /* we can adjust `charcode', otherwise the whole segment is */
            /* invalid                                                  */
            gindex = 0;

            if ( (FT_Int)charcode + delta < 0 &&
                 (FT_Int)end + delta >= 0     )
              charcode = (FT_UInt)( -delta );

            else if ( (FT_Int)charcode + delta < 0x10000L &&
                      (FT_Int)end + delta >= 0x10000L     )
              charcode = (FT_UInt)( 0x10000L - delta );

            else
              continue;
          }
        }

        if ( next && !gindex )
        {
          if ( charcode >= 0xFFFFU )
            break;

          charcode++;
          goto Again;
        }

        break;
      }
    }

    if ( next )
      *pcharcode = charcode;

    return gindex;
  }

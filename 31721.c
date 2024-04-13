  tt_cmap0_validate( FT_Byte*      table,
                     FT_Validator  valid )
  {
    FT_Byte*  p;
    FT_UInt   length;


    if ( table + 2 + 2 > valid->limit )
      FT_INVALID_TOO_SHORT;

    p      = table + 2;           /* skip format */
    length = TT_NEXT_USHORT( p );

    if ( table + length > valid->limit || length < 262 )
      FT_INVALID_TOO_SHORT;

    /* check glyph indices whenever necessary */
    if ( valid->level >= FT_VALIDATE_TIGHT )
    {
      FT_UInt  n, idx;


      p = table + 6;
      for ( n = 0; n < 256; n++ )
      {
        idx = *p++;
        if ( idx >= TT_VALID_GLYPH_COUNT( valid ) )
          FT_INVALID_GLYPH_ID;
      }
    }

    return FT_Err_Ok;
  }

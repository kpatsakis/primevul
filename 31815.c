  tt_cmap14_char_map_def_binary( FT_Byte    *base,
                                 FT_UInt32   char_code )
  {
    FT_UInt32  numRanges = TT_PEEK_ULONG( base );
    FT_UInt32  max, min;


    min = 0;
    max = numRanges;

    base += 4;

    /* binary search */
    while ( min < max )
    {
      FT_UInt32  mid   = ( min + max ) >> 1;
      FT_Byte*   p     = base + 4 * mid;
      FT_ULong   start = TT_NEXT_UINT24( p );
      FT_UInt    cnt   = FT_NEXT_BYTE( p );


      if ( char_code < start )
        max = mid;
      else if ( char_code > start+cnt )
        min = mid + 1;
      else
        return TRUE;
    }

    return FALSE;
  }

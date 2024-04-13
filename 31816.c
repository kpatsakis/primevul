  tt_cmap14_char_map_nondef_binary( FT_Byte    *base,
                                    FT_UInt32   char_code )
  {
    FT_UInt32  numMappings = TT_PEEK_ULONG( base );
    FT_UInt32  max, min;


    min = 0;
    max = numMappings;

    base += 4;

    /* binary search */
    while ( min < max )
    {
      FT_UInt32  mid = ( min + max ) >> 1;
      FT_Byte*   p   = base + 5 * mid;
      FT_UInt32  uni = (FT_UInt32)TT_NEXT_UINT24( p );


      if ( char_code < uni )
        max = mid;
      else if ( char_code > uni )
        min = mid + 1;
      else
        return TT_PEEK_USHORT( p );
    }

    return 0;
  }

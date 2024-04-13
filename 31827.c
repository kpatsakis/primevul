  tt_cmap14_get_nondef_chars( TT_CMap     cmap,
                              FT_Byte    *p,
                              FT_Memory   memory )
  {
    TT_CMap14   cmap14 = (TT_CMap14) cmap;
    FT_UInt32   numMappings;
    FT_UInt     i;
    FT_UInt32  *ret;


    numMappings = (FT_UInt32)TT_NEXT_ULONG( p );

    if ( tt_cmap14_ensure( cmap14, ( numMappings + 1 ), memory ) )
      return NULL;

    ret = cmap14->results;
    for ( i = 0; i < numMappings; ++i )
    {
      ret[i] = (FT_UInt32)TT_NEXT_UINT24( p );
      p += 2;
    }
    ret[i] = 0;

    return ret;
  }

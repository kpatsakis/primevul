  tt_cmap14_variants( TT_CMap    cmap,
                      FT_Memory  memory )
  {
    TT_CMap14   cmap14 = (TT_CMap14)cmap;
    FT_UInt32   count  = cmap14->num_selectors;
    FT_Byte*    p      = cmap->data + 10;
    FT_UInt32*  result;
    FT_UInt32   i;


    if ( tt_cmap14_ensure( cmap14, ( count + 1 ), memory ) )
      return NULL;

    result = cmap14->results;
    for ( i = 0; i < count; ++i )
    {
      result[i] = (FT_UInt32)TT_NEXT_UINT24( p );
      p        += 8;
    }
    result[i] = 0;

    return result;
  }

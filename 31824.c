  tt_cmap14_find_variant( FT_Byte    *base,
                          FT_UInt32   variantCode )
  {
    FT_UInt32  numVar = TT_PEEK_ULONG( base );
    FT_UInt32  max, min;


    min = 0;
    max = numVar;

    base += 4;

    /* binary search */
    while ( min < max )
    {
      FT_UInt32  mid    = ( min + max ) >> 1;
      FT_Byte*   p      = base + 11 * mid;
      FT_ULong   varSel = TT_NEXT_UINT24( p );


      if ( variantCode < varSel )
        max = mid;
      else if ( variantCode > varSel )
        min = mid + 1;
      else
        return p;
    }

    return NULL;
  }

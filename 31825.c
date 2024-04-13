  tt_cmap14_get_def_chars( TT_CMap    cmap,
                           FT_Byte*   p,
                           FT_Memory  memory )
  {
    TT_CMap14   cmap14 = (TT_CMap14) cmap;
    FT_UInt32   numRanges;
    FT_UInt     cnt;
    FT_UInt32*  q;


    cnt       = tt_cmap14_def_char_count( p );
    numRanges = (FT_UInt32)TT_NEXT_ULONG( p );

    if ( tt_cmap14_ensure( cmap14, ( cnt + 1 ), memory ) )
      return NULL;

    for ( q = cmap14->results; numRanges > 0; --numRanges )
    {
      FT_UInt32  uni = (FT_UInt32)TT_NEXT_UINT24( p );


      cnt = FT_NEXT_BYTE( p ) + 1;
      do
      {
        q[0]  = uni;
        uni  += 1;
        q    += 1;

      } while ( --cnt != 0 );
    }
    q[0] = 0;

    return cmap14->results;
  }

  tt_cmap14_char_variants( TT_CMap    cmap,
                           FT_Memory  memory,
                           FT_UInt32  charCode )
  {
    TT_CMap14   cmap14 = (TT_CMap14)  cmap;
    FT_UInt32   count  = cmap14->num_selectors;
    FT_Byte*    p      = cmap->data + 10;
    FT_UInt32*  q;


    if ( tt_cmap14_ensure( cmap14, ( count + 1 ), memory ) )
      return NULL;

    for ( q = cmap14->results; count > 0; --count )
    {
      FT_UInt32  varSel    = TT_NEXT_UINT24( p );
      FT_ULong   defOff    = TT_NEXT_ULONG( p );
      FT_ULong   nondefOff = TT_NEXT_ULONG( p );


      if ( ( defOff != 0                                               &&
             tt_cmap14_char_map_def_binary( cmap->data + defOff,
                                            charCode )                 ) ||
           ( nondefOff != 0                                            &&
             tt_cmap14_char_map_nondef_binary( cmap->data + nondefOff,
                                               charCode ) != 0         ) )
      {
        q[0] = varSel;
        q++;
      }
    }
    q[0] = 0;

    return cmap14->results;
  }

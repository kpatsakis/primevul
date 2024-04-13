  tt_cmap14_char_var_isdefault( TT_CMap    cmap,
                                FT_UInt32  charcode,
                                FT_UInt32  variantSelector )
  {
    FT_Byte*  p = tt_cmap14_find_variant( cmap->data + 6, variantSelector );
    FT_ULong  defOff;
    FT_ULong  nondefOff;


    if ( !p )
      return -1;

    defOff    = TT_NEXT_ULONG( p );
    nondefOff = TT_NEXT_ULONG( p );

    if ( defOff != 0                                                    &&
         tt_cmap14_char_map_def_binary( cmap->data + defOff, charcode ) )
      return 1;

    if ( nondefOff != 0                                            &&
         tt_cmap14_char_map_nondef_binary( cmap->data + nondefOff,
                                           charcode ) != 0         )
      return 0;

    return -1;
  }

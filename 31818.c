  tt_cmap14_char_var_index( TT_CMap    cmap,
                            TT_CMap    ucmap,
                            FT_UInt32  charcode,
                            FT_UInt32  variantSelector )
  {
    FT_Byte*  p = tt_cmap14_find_variant( cmap->data + 6, variantSelector );
    FT_ULong  defOff;
    FT_ULong  nondefOff;


    if ( !p )
      return 0;

    defOff    = TT_NEXT_ULONG( p );
    nondefOff = TT_PEEK_ULONG( p );

    if ( defOff != 0                                                    &&
         tt_cmap14_char_map_def_binary( cmap->data + defOff, charcode ) )
    {
      /* This is the default variant of this charcode.  GID not stored */
      /* here; stored in the normal Unicode charmap instead.           */
      return ucmap->cmap.clazz->char_index( &ucmap->cmap, charcode );
    }

    if ( nondefOff != 0 )
      return tt_cmap14_char_map_nondef_binary( cmap->data + nondefOff,
                                               charcode );

    return 0;
  }

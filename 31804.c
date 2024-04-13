  tt_cmap12_char_next( TT_CMap     cmap,
                       FT_UInt32  *pchar_code )
  {
    TT_CMap12  cmap12 = (TT_CMap12)cmap;
    FT_ULong   gindex;


    if ( cmap12->cur_charcode >= 0xFFFFFFFFUL )
      return 0;

    /* no need to search */
    if ( cmap12->valid && cmap12->cur_charcode == *pchar_code )
    {
      tt_cmap12_next( cmap12 );
      if ( cmap12->valid )
      {
        gindex = cmap12->cur_gindex;

        /* XXX: check cur_charcode overflow is expected */
        if ( gindex )
          *pchar_code = (FT_UInt32)cmap12->cur_charcode;
      }
      else
        gindex = 0;
    }
    else
      gindex = tt_cmap12_char_map_binary( cmap, pchar_code, 1 );

    /* XXX: check gindex overflow is expected */
    return (FT_UInt32)gindex;
  }

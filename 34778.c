  tt_cmap13_char_next( TT_CMap     cmap,
                       FT_UInt32  *pchar_code )
  {
    TT_CMap13  cmap13 = (TT_CMap13)cmap;
    FT_UInt    gindex;


    /* no need to search */
    if ( cmap13->valid && cmap13->cur_charcode == *pchar_code )
    {
      tt_cmap13_next( cmap13 );
      if ( cmap13->valid )
      {
        gindex      = cmap13->cur_gindex;
        *pchar_code = cmap13->cur_charcode;
      }
      else
        gindex = 0;
    }
    else
      gindex = tt_cmap13_char_map_binary( cmap, pchar_code, 1 );

    return gindex;
  }

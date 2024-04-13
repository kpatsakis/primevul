  tt_cmap14_char_next( TT_CMap     cmap,
                       FT_UInt32  *pchar_code )
  {
    FT_UNUSED( cmap );

    /* This can't happen */
    *pchar_code = 0;
    return 0;
  }

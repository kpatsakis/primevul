  Ins_GETINFO( INS_ARG )
  {
    FT_Long  K;


    K = 0;

    /* We return MS rasterizer version 1.7 for the font scaler. */
    if ( ( args[0] & 1 ) != 0 )
      K = 35;

    /* Has the glyph been rotated? */
    if ( ( args[0] & 2 ) != 0 && CUR.tt_metrics.rotated )
      K |= 0x80;

    /* Has the glyph been stretched? */
    if ( ( args[0] & 4 ) != 0 && CUR.tt_metrics.stretched )
      K |= 1 << 8;

    /* Are we hinting for grayscale? */
    if ( ( args[0] & 32 ) != 0 && CUR.grayscale )
      K |= 1 << 12;

    args[0] = K;
  }

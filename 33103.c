  TT_DotFix14_long_long( FT_Int32  ax,
                         FT_Int32  ay,
                         FT_Int    bx,
                         FT_Int    by )
  {
    /* Temporarily disable the warning that C90 doesn't support */
    /* `long long'.                                             */

    long long  temp1 = (long long)ax * bx;
    long long  temp2 = (long long)ay * by;


    temp1 += temp2;
    temp2  = temp1 >> 63;
    temp1 += 0x2000 + temp2;

    return (FT_Int32)( temp1 >> 14 );

  }

  tt_cmap14_def_char_count( FT_Byte  *p )
  {
    FT_UInt32  numRanges = (FT_UInt32)TT_NEXT_ULONG( p );
    FT_UInt    tot       = 0;


    p += 3;  /* point to the first `cnt' field */
    for ( ; numRanges > 0; numRanges-- )
    {
      tot += 1 + p[0];
      p   += 4;
    }

    return tot;
  }

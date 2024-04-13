  Ins_WCVTF( TT_ExecContext  exc,
             FT_Long*        args )
  {
    FT_ULong  I = (FT_ULong)args[0];


    if ( BOUNDSL( I, exc->cvtSize ) )
    {
      if ( exc->pedantic_hinting )
        ARRAY_BOUND_ERROR;
    }
    else
      exc->cvt[I] = FT_MulFix( args[1], exc->tt_metrics.scale );
  }

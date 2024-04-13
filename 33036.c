  Ins_RCVT( TT_ExecContext  exc,
            FT_Long*        args )
  {
    FT_ULong  I = (FT_ULong)args[0];


    if ( BOUNDSL( I, exc->cvtSize ) )
    {
      if ( exc->pedantic_hinting )
        ARRAY_BOUND_ERROR;
      else
        args[0] = 0;
    }
    else
      args[0] = exc->func_read_cvt( exc, I );
  }

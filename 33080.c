  Ins_WCVTP( TT_ExecContext  exc,
             FT_Long*        args )
  {
    FT_ULong  I = (FT_ULong)args[0];


    if ( BOUNDSL( I, exc->cvtSize ) )
    {
      if ( exc->pedantic_hinting )
        ARRAY_BOUND_ERROR;
    }
    else
      exc->func_write_cvt( exc, I, args[1] );
  }

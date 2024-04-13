  Ins_WS( TT_ExecContext  exc,
          FT_Long*        args )
  {
    FT_ULong  I = (FT_ULong)args[0];


    if ( BOUNDSL( I, exc->storeSize ) )
    {
      if ( exc->pedantic_hinting )
        ARRAY_BOUND_ERROR;
    }
    else
      exc->storage[I] = args[1];
  }

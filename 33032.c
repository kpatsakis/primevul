  Ins_ODD( TT_ExecContext  exc,
           FT_Long*        args )
  {
    args[0] = ( ( exc->func_round( exc, args[0], 0 ) & 127 ) == 64 );
  }

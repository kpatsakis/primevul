  Ins_DIV( TT_ExecContext  exc,
           FT_Long*        args )
  {
    if ( args[1] == 0 )
      exc->error = FT_THROW( Divide_By_Zero );
    else
      args[0] = FT_MulDiv_No_Round( args[0], 64L, args[1] );
  }

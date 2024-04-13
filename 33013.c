  Ins_JROT( TT_ExecContext  exc,
            FT_Long*        args )
  {
    if ( args[1] != 0 )
      Ins_JMPR( exc, args );
  }

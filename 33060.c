  Ins_SLOOP( TT_ExecContext  exc,
             FT_Long*        args )
  {
    if ( args[0] < 0 )
      exc->error = FT_THROW( Bad_Argument );
    else
      exc->GS.loop = args[0];
  }

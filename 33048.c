  Ins_SCANTYPE( TT_ExecContext  exc,
                FT_Long*        args )
  {
    if ( args[0] >= 0 )
      exc->GS.scan_type = (FT_Int)args[0] & 0xFFFF;
  }

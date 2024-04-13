  Ins_SDB( TT_ExecContext  exc,
           FT_Long*        args )
  {
    exc->GS.delta_base = (FT_UShort)args[0];
  }

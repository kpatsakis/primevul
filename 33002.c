  Ins_GPV( TT_ExecContext  exc,
           FT_Long*        args )
  {
    args[0] = exc->GS.projVector.x;
    args[1] = exc->GS.projVector.y;
  }

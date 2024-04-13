  Ins_GFV( TT_ExecContext  exc,
           FT_Long*        args )
  {
    args[0] = exc->GS.freeVector.x;
    args[1] = exc->GS.freeVector.y;
  }

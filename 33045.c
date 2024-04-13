  Ins_S45ROUND( TT_ExecContext  exc,
                FT_Long*        args )
  {
    SetSuperRound( exc, 0x2D41, args[0] );

    exc->GS.round_state = TT_Round_Super_45;
    exc->func_round     = (TT_Round_Func)Round_Super_45;
  }

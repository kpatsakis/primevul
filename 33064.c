  Ins_SROUND( TT_ExecContext  exc,
              FT_Long*        args )
  {
    SetSuperRound( exc, 0x4000, args[0] );

    exc->GS.round_state = TT_Round_Super;
    exc->func_round     = (TT_Round_Func)Round_Super;
  }

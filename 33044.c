  Ins_RUTG( TT_ExecContext  exc )
  {
    exc->GS.round_state = TT_Round_Up_To_Grid;
    exc->func_round     = (TT_Round_Func)Round_Up_To_Grid;
  }

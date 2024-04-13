  Ins_RTG( TT_ExecContext  exc )
  {
    exc->GS.round_state = TT_Round_To_Grid;
    exc->func_round     = (TT_Round_Func)Round_To_Grid;
  }

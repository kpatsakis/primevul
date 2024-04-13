  Ins_RDTG( TT_ExecContext  exc )
  {
    exc->GS.round_state = TT_Round_Down_To_Grid;
    exc->func_round     = (TT_Round_Func)Round_Down_To_Grid;
  }

  Compute_Round( TT_ExecContext  exc,
                 FT_Byte         round_mode )
  {
    switch ( round_mode )
    {
    case TT_Round_Off:
      exc->func_round = (TT_Round_Func)Round_None;
      break;

    case TT_Round_To_Grid:
      exc->func_round = (TT_Round_Func)Round_To_Grid;
      break;

    case TT_Round_Up_To_Grid:
      exc->func_round = (TT_Round_Func)Round_Up_To_Grid;
      break;

    case TT_Round_Down_To_Grid:
      exc->func_round = (TT_Round_Func)Round_Down_To_Grid;
      break;

    case TT_Round_To_Half_Grid:
      exc->func_round = (TT_Round_Func)Round_To_Half_Grid;
      break;

    case TT_Round_To_Double_Grid:
      exc->func_round = (TT_Round_Func)Round_To_Double_Grid;
      break;

    case TT_Round_Super:
      exc->func_round = (TT_Round_Func)Round_Super;
      break;

    case TT_Round_Super_45:
      exc->func_round = (TT_Round_Func)Round_Super_45;
      break;
    }
  }

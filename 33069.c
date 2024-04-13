  Ins_SSWCI( TT_ExecContext  exc,
             FT_Long*        args )
  {
    exc->GS.single_width_cutin = (FT_F26Dot6)args[0];
  }

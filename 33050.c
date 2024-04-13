  Ins_SCVTCI( TT_ExecContext  exc,
              FT_Long*        args )
  {
    exc->GS.control_value_cutin = (FT_F26Dot6)args[0];
  }

  Write_CVT( TT_ExecContext  exc,
             FT_ULong        idx,
             FT_F26Dot6      value )
  {
    exc->cvt[idx] = value;
  }

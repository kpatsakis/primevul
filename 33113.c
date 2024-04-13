  Write_CVT_Stretched( TT_ExecContext  exc,
                       FT_ULong        idx,
                       FT_F26Dot6      value )
  {
    exc->cvt[idx] = FT_DivFix( value, Current_Ratio( exc ) );
  }

  Read_CVT_Stretched( TT_ExecContext  exc,
                      FT_ULong        idx )
  {
    return FT_MulFix( exc->cvt[idx], Current_Ratio( exc ) );
  }

  Current_Ppem_Stretched( TT_ExecContext  exc )
  {
    return FT_MulFix( exc->tt_metrics.ppem, Current_Ratio( exc ) );
  }

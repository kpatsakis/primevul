  Ins_SSW( TT_ExecContext  exc,
           FT_Long*        args )
  {
    exc->GS.single_width_value = FT_MulFix( args[0],
                                            exc->tt_metrics.scale );
  }

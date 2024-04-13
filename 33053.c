  Ins_SDS( TT_ExecContext  exc,
           FT_Long*        args )
  {
    if ( (FT_ULong)args[0] > 6UL )
      exc->error = FT_THROW( Bad_Argument );
    else
      exc->GS.delta_shift = (FT_UShort)args[0];
  }

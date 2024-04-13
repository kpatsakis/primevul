  Ins_SPVTL( TT_ExecContext  exc,
             FT_Long*        args )
  {
    if ( Ins_SxVTL( exc,
                    (FT_UShort)args[1],
                    (FT_UShort)args[0],
                    &exc->GS.projVector ) == SUCCESS )
    {
      exc->GS.dualVector = exc->GS.projVector;
      Compute_Funcs( exc );
    }
  }

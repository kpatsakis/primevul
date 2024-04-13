  Ins_SFVTL( TT_ExecContext  exc,
             FT_Long*        args )
  {
    if ( Ins_SxVTL( exc,
                    (FT_UShort)args[1],
                    (FT_UShort)args[0],
                    &exc->GS.freeVector ) == SUCCESS )
    {
      Compute_Funcs( exc );
    }
  }

  Ins_SFVTPV( TT_ExecContext  exc )
  {
    exc->GS.freeVector = exc->GS.projVector;
    Compute_Funcs( exc );
  }

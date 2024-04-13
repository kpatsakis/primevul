  Ins_SPVFS( TT_ExecContext  exc,
             FT_Long*        args )
  {
    FT_Short  S;
    FT_Long   X, Y;


    /* Only use low 16bits, then sign extend */
    S = (FT_Short)args[1];
    Y = (FT_Long)S;
    S = (FT_Short)args[0];
    X = (FT_Long)S;

    Normalize( X, Y, &exc->GS.projVector );

    exc->GS.dualVector = exc->GS.projVector;
    Compute_Funcs( exc );
  }

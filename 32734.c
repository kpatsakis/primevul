  Ins_MINDEX( INS_ARG )
  {
    FT_Long  L, K;


    L = args[0];

    if ( L <= 0 || L > CUR.args )
    {
      CUR.error = TT_Err_Invalid_Reference;
      return;
    }

    K = CUR.stack[CUR.args - L];

    FT_ARRAY_MOVE( &CUR.stack[CUR.args - L    ],
                   &CUR.stack[CUR.args - L + 1],
                   ( L - 1 ) );

    CUR.stack[CUR.args - 1] = K;
  }

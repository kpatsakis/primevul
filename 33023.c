  Ins_MINDEX( TT_ExecContext  exc,
              FT_Long*        args )
  {
    FT_Long  L, K;


    L = args[0];

    if ( L <= 0 || L > exc->args )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
    }
    else
    {
      K = exc->stack[exc->args - L];

      FT_ARRAY_MOVE( &exc->stack[exc->args - L    ],
                     &exc->stack[exc->args - L + 1],
                     ( L - 1 ) );

      exc->stack[exc->args - 1] = K;
    }
  }

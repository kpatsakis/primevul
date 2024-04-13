  Ins_CINDEX( TT_ExecContext  exc,
              FT_Long*        args )
  {
    FT_Long  L;


    L = args[0];

    if ( L <= 0 || L > exc->args )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      args[0] = 0;
    }
    else
      args[0] = exc->stack[exc->args - L];
  }

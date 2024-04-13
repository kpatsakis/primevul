  Ins_PUSHB( TT_ExecContext  exc,
             FT_Long*        args )
  {
    FT_UShort  L, K;


    L = (FT_UShort)( exc->opcode - 0xB0 + 1 );

    if ( BOUNDS( L, exc->stackSize + 1 - exc->top ) )
    {
      exc->error = FT_THROW( Stack_Overflow );
      return;
    }

    for ( K = 1; K <= L; K++ )
      args[K - 1] = exc->code[exc->IP + K];
  }

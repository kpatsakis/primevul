  Ins_NPUSHW( TT_ExecContext  exc,
              FT_Long*        args )
  {
    FT_UShort  L, K;


    L = (FT_UShort)exc->code[exc->IP + 1];

    if ( BOUNDS( L, exc->stackSize + 1 - exc->top ) )
    {
      exc->error = FT_THROW( Stack_Overflow );
      return;
    }

    exc->IP += 2;

    for ( K = 0; K < L; K++ )
      args[K] = GetShortIns( exc );

    exc->step_ins = FALSE;
    exc->new_top += L;
  }

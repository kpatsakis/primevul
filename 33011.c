  Ins_JMPR( TT_ExecContext  exc,
            FT_Long*        args )
  {
    if ( args[0] == 0 && exc->args == 0 )
    {
      exc->error = FT_THROW( Bad_Argument );
      return;
    }

    exc->IP += args[0];
    if ( exc->IP < 0                                             ||
         ( exc->callTop > 0                                    &&
           exc->IP > exc->callStack[exc->callTop - 1].Def->end ) )
    {
      exc->error = FT_THROW( Bad_Argument );
      return;
    }

    exc->step_ins = FALSE;

    if ( args[0] < 0 )
    {
      if ( ++exc->neg_jump_counter > exc->neg_jump_counter_max )
        exc->error = FT_THROW( Execution_Too_Long );
    }
  }

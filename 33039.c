  Ins_ROUND( TT_ExecContext  exc,
             FT_Long*        args )
  {
    args[0] = exc->func_round(
                exc,
                args[0],
                exc->tt_metrics.compensations[exc->opcode - 0x68] );
  }

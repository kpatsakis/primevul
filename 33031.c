  Ins_NROUND( TT_ExecContext  exc,
              FT_Long*        args )
  {
    args[0] = Round_None(
                exc,
                args[0],
                exc->tt_metrics.compensations[exc->opcode - 0x6C] );
  }

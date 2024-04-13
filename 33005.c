  Ins_IDEF( TT_ExecContext  exc,
            FT_Long*        args )
  {
    TT_DefRecord*  def;
    TT_DefRecord*  limit;


    /* we enable IDEF only in `prep' or `fpgm' */
    if ( exc->curRange == tt_coderange_glyph )
    {
      exc->error = FT_THROW( DEF_In_Glyf_Bytecode );
      return;
    }

    /*  First of all, look for the same function in our table */

    def   = exc->IDefs;
    limit = def + exc->numIDefs;

    for ( ; def < limit; def++ )
      if ( def->opc == (FT_ULong)args[0] )
        break;

    if ( def == limit )
    {
      /* check that there is enough room for a new instruction */
      if ( exc->numIDefs >= exc->maxIDefs )
      {
        exc->error = FT_THROW( Too_Many_Instruction_Defs );
        return;
      }
      exc->numIDefs++;
    }

    /* opcode must be unsigned 8-bit integer */
    if ( 0 > args[0] || args[0] > 0x00FF )
    {
      exc->error = FT_THROW( Too_Many_Instruction_Defs );
      return;
    }

    def->opc    = (FT_Byte)args[0];
    def->start  = exc->IP + 1;
    def->range  = exc->curRange;
    def->active = TRUE;

    if ( (FT_ULong)args[0] > exc->maxIns )
      exc->maxIns = (FT_Byte)args[0];

    /* Now skip the whole function definition. */
    /* We don't allow nested IDEFs & FDEFs.    */

    while ( SkipCode( exc ) == SUCCESS )
    {
      switch ( exc->opcode )
      {
      case 0x89:   /* IDEF */
      case 0x2C:   /* FDEF */
        exc->error = FT_THROW( Nested_DEFS );
        return;
      case 0x2D:   /* ENDF */
        def->end = exc->IP;
        return;
      }
    }
  }

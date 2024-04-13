  Ins_ELSE( TT_ExecContext  exc )
  {
    FT_Int  nIfs;


    nIfs = 1;

    do
    {
      if ( SkipCode( exc ) == FAILURE )
        return;

      switch ( exc->opcode )
      {
      case 0x58:    /* IF */
        nIfs++;
        break;

      case 0x59:    /* EIF */
        nIfs--;
        break;
      }
    } while ( nIfs != 0 );
  }

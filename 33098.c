  SetSuperRound( TT_ExecContext  exc,
                 FT_F2Dot14      GridPeriod,
                 FT_Long         selector )
  {
    switch ( (FT_Int)( selector & 0xC0 ) )
    {
      case 0:
        exc->period = GridPeriod / 2;
        break;

      case 0x40:
        exc->period = GridPeriod;
        break;

      case 0x80:
        exc->period = GridPeriod * 2;
        break;

      /* This opcode is reserved, but... */
      case 0xC0:
        exc->period = GridPeriod;
        break;
    }

    switch ( (FT_Int)( selector & 0x30 ) )
    {
    case 0:
      exc->phase = 0;
      break;

    case 0x10:
      exc->phase = exc->period / 4;
      break;

    case 0x20:
      exc->phase = exc->period / 2;
      break;

    case 0x30:
      exc->phase = exc->period * 3 / 4;
      break;
    }

    if ( ( selector & 0x0F ) == 0 )
      exc->threshold = exc->period - 1;
    else
      exc->threshold = ( (FT_Int)( selector & 0x0F ) - 4 ) * exc->period / 8;

    /* convert to F26Dot6 format */
    exc->period    >>= 8;
    exc->phase     >>= 8;
    exc->threshold >>= 8;
  }

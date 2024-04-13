  Ins_DELTAC( TT_ExecContext  exc,
              FT_Long*        args )
  {
    FT_ULong  nump, k;
    FT_ULong  A, C, P;
    FT_Long   B;


    P    = (FT_ULong)exc->func_cur_ppem( exc );
    nump = (FT_ULong)args[0];

    for ( k = 1; k <= nump; k++ )
    {
      if ( exc->args < 2 )
      {
        if ( exc->pedantic_hinting )
          exc->error = FT_THROW( Too_Few_Arguments );
        exc->args = 0;
        goto Fail;
      }

      exc->args -= 2;

      A = (FT_ULong)exc->stack[exc->args + 1];
      B = exc->stack[exc->args];

      if ( BOUNDSL( A, exc->cvtSize ) )
      {
        if ( exc->pedantic_hinting )
        {
          exc->error = FT_THROW( Invalid_Reference );
          return;
        }
      }
      else
      {
        C = ( (FT_ULong)B & 0xF0 ) >> 4;

        switch ( exc->opcode )
        {
        case 0x73:
          break;

        case 0x74:
          C += 16;
          break;

        case 0x75:
          C += 32;
          break;
        }

        C += exc->GS.delta_base;

        if ( P == C )
        {
          B = ( (FT_ULong)B & 0xF ) - 8;
          if ( B >= 0 )
            B++;
          B *= 1L << ( 6 - exc->GS.delta_shift );

          exc->func_move_cvt( exc, A, B );
        }
      }
    }

  Fail:
    exc->new_top = exc->args;
  }

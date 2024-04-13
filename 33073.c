  Ins_SZP1( TT_ExecContext  exc,
            FT_Long*        args )
  {
    switch ( (FT_Int)args[0] )
    {
    case 0:
      exc->zp1 = exc->twilight;
      break;

    case 1:
      exc->zp1 = exc->pts;
      break;

    default:
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      return;
    }

    exc->GS.gep1 = (FT_UShort)args[0];
  }

  Ins_SZP2( TT_ExecContext  exc,
            FT_Long*        args )
  {
    switch ( (FT_Int)args[0] )
    {
    case 0:
      exc->zp2 = exc->twilight;
      break;

    case 1:
      exc->zp2 = exc->pts;
      break;

    default:
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      return;
    }

    exc->GS.gep2 = (FT_UShort)args[0];
  }

  Ins_SZP0( TT_ExecContext  exc,
            FT_Long*        args )
  {
    switch ( (FT_Int)args[0] )
    {
    case 0:
      exc->zp0 = exc->twilight;
      break;

    case 1:
      exc->zp0 = exc->pts;
      break;

    default:
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      return;
    }

    exc->GS.gep0 = (FT_UShort)args[0];
  }

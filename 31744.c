  tt_sbit_decoder_load_metrics( TT_SBitDecoder  decoder,
                                FT_Byte*       *pp,
                                FT_Byte*        limit,
                                FT_Bool         big )
  {
    FT_Byte*         p       = *pp;
    TT_SBit_Metrics  metrics = decoder->metrics;


    if ( p + 5 > limit )
      goto Fail;

    metrics->height       = p[0];
    metrics->width        = p[1];
    metrics->horiBearingX = (FT_Char)p[2];
    metrics->horiBearingY = (FT_Char)p[3];
    metrics->horiAdvance  = p[4];

    p += 5;
    if ( big )
    {
      if ( p + 3 > limit )
        goto Fail;

      metrics->vertBearingX = (FT_Char)p[0];
      metrics->vertBearingY = (FT_Char)p[1];
      metrics->vertAdvance  = p[2];

      p += 3;
    }
    else
    {
      /* avoid uninitialized data in case there is no vertical info -- */
      metrics->vertBearingX = 0;
      metrics->vertBearingY = 0;
      metrics->vertAdvance  = 0;
    }

    decoder->metrics_loaded = 1;
    *pp = p;
    return FT_Err_Ok;

  Fail:
    FT_TRACE1(( "tt_sbit_decoder_load_metrics: broken table\n" ));
    return FT_THROW( Invalid_Argument );
  }

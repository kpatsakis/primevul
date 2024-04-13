  cff_charset_compute_cids( CFF_Charset  charset,
                            FT_UInt      num_glyphs,
                            FT_Memory    memory )
  {
    FT_Error   error   = FT_Err_Ok;
    FT_UInt    i;
    FT_UShort  max_cid = 0;


    if ( charset->max_cid > 0 )
      goto Exit;

    for ( i = 0; i < num_glyphs; i++ )
      if ( charset->sids[i] > max_cid )
        max_cid = charset->sids[i];
    max_cid++;

    if ( FT_NEW_ARRAY( charset->cids, max_cid ) )
      goto Exit;

    for ( i = 0; i < num_glyphs; i++ )
      charset->cids[charset->sids[i]] = (FT_UShort)i;

    charset->max_cid    = max_cid;
    charset->num_glyphs = num_glyphs;

  Exit:
    return error;
  }

  cff_charset_done( CFF_Charset  charset,
                    FT_Stream    stream )
  {
    FT_Memory  memory = stream->memory;


    cff_charset_free_cids( charset, memory );

    FT_FREE( charset->sids );
    charset->format = 0;
    charset->offset = 0;
  }

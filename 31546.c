  parse_private( T1_Face    face,
                 T1_Loader  loader )
  {
    FT_UNUSED( face );

    loader->keywords_encountered |= T1_PRIVATE;
  }

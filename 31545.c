  parse_buildchar( T1_Face    face,
                   T1_Loader  loader )
  {
    face->len_buildchar = T1_ToFixedArray( &loader->parser, 0, NULL, 0 );

    return;
  }

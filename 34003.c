  cff_parser_done( CFF_Parser  parser )
  {
    FT_Memory  memory = parser->library->memory;    /* for FT_FREE */


    FT_FREE( parser->stack );
  }

  tt_size_done_bytecode( FT_Size  ftsize )
  {
    TT_Size    size   = (TT_Size)ftsize;
    TT_Face    face   = (TT_Face)ftsize->face;
    FT_Memory  memory = face->root.memory;

    if ( size->context )
    {
      TT_Done_Context( size->context );
      size->context = NULL;
    }

    FT_FREE( size->cvt );
    size->cvt_size = 0;

    /* free storage area */
    FT_FREE( size->storage );
    size->storage_size = 0;

    /* twilight zone */
    tt_glyphzone_done( &size->twilight );

    FT_FREE( size->function_defs );
    FT_FREE( size->instruction_defs );

    size->num_function_defs    = 0;
    size->max_function_defs    = 0;
    size->num_instruction_defs = 0;
    size->max_instruction_defs = 0;

    size->max_func = 0;
    size->max_ins  = 0;

    size->bytecode_ready = -1;
    size->cvt_ready      = -1;
  }

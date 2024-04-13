  tt_size_init_bytecode( FT_Size  ftsize,
                         FT_Bool  pedantic )
  {
    FT_Error   error;
    TT_Size    size = (TT_Size)ftsize;
    TT_Face    face = (TT_Face)ftsize->face;
    FT_Memory  memory = face->root.memory;

    FT_UShort       n_twilight;
    TT_MaxProfile*  maxp = &face->max_profile;


    /* clean up bytecode related data */
    FT_FREE( size->function_defs );
    FT_FREE( size->instruction_defs );
    FT_FREE( size->cvt );
    FT_FREE( size->storage );

    if ( size->context )
      TT_Done_Context( size->context );
    tt_glyphzone_done( &size->twilight );

    size->bytecode_ready = -1;
    size->cvt_ready      = -1;

    size->context = TT_New_Context( (TT_Driver)face->root.driver );

    size->max_function_defs    = maxp->maxFunctionDefs;
    size->max_instruction_defs = maxp->maxInstructionDefs;

    size->num_function_defs    = 0;
    size->num_instruction_defs = 0;

    size->max_func = 0;
    size->max_ins  = 0;

    size->cvt_size     = face->cvt_size;
    size->storage_size = maxp->maxStorage;

    /* Set default metrics */
    {
      TT_Size_Metrics*  metrics = &size->ttmetrics;


      metrics->rotated   = FALSE;
      metrics->stretched = FALSE;

      /* set default engine compensation */
      metrics->compensations[0] = 0;   /* gray     */
      metrics->compensations[1] = 0;   /* black    */
      metrics->compensations[2] = 0;   /* white    */
      metrics->compensations[3] = 0;   /* reserved */
    }

    /* allocate function defs, instruction defs, cvt, and storage area */
    if ( FT_NEW_ARRAY( size->function_defs,    size->max_function_defs    ) ||
         FT_NEW_ARRAY( size->instruction_defs, size->max_instruction_defs ) ||
         FT_NEW_ARRAY( size->cvt,              size->cvt_size             ) ||
         FT_NEW_ARRAY( size->storage,          size->storage_size         ) )
      goto Exit;

    /* reserve twilight zone */
    n_twilight = maxp->maxTwilightPoints;

    /* there are 4 phantom points (do we need this?) */
    n_twilight += 4;

    error = tt_glyphzone_new( memory, n_twilight, 0, &size->twilight );
    if ( error )
      goto Exit;

    size->twilight.n_points = n_twilight;

    size->GS = tt_default_graphics_state;

    /* set `face->interpreter' according to the debug hook present */
    {
      FT_Library  library = face->root.driver->root.library;


      face->interpreter = (TT_Interpreter)
                            library->debug_hooks[FT_DEBUG_HOOK_TRUETYPE];
      if ( !face->interpreter )
        face->interpreter = (TT_Interpreter)TT_RunIns;
    }

    /* Fine, now run the font program! */

    /* In case of an error while executing `fpgm', we intentionally don't */
    /* clean up immediately – bugs in the `fpgm' are so fundamental that  */
    /* all following hinting calls should fail.  Additionally, `fpgm' is  */
    /* to be executed just once; calling it again is completely useless   */
    /* and might even lead to extremely slow behaviour if it is malformed */
    /* (containing an infinite loop, for example).                        */
    error = tt_size_run_fpgm( size, pedantic );
    return error;

  Exit:
    if ( error )
      tt_size_done_bytecode( ftsize );

    return error;
  }

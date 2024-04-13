  Init_Context( TT_ExecContext  exec,
                FT_Memory       memory )
  {
    FT_Error  error;


    FT_TRACE1(( "Init_Context: new object at 0x%08p\n", exec ));

    exec->memory   = memory;
    exec->callSize = 32;

    if ( FT_NEW_ARRAY( exec->callStack, exec->callSize ) )
      goto Fail_Memory;

    /* all values in the context are set to 0 already, but this is */
    /* here as a remainder                                         */
    exec->maxPoints   = 0;
    exec->maxContours = 0;

    exec->stackSize = 0;
    exec->glyphSize = 0;

    exec->stack    = NULL;
    exec->glyphIns = NULL;

    exec->face = NULL;
    exec->size = NULL;

    return FT_Err_Ok;

  Fail_Memory:
    FT_ERROR(( "Init_Context: not enough memory for %p\n", exec ));
    TT_Done_Context( exec );

    return error;
 }

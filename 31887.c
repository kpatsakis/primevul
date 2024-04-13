  tt_size_init( FT_Size  ttsize )           /* TT_Size */
  {
    TT_Size   size  = (TT_Size)ttsize;
    FT_Error  error = FT_Err_Ok;


#ifdef TT_USE_BYTECODE_INTERPRETER
    size->bytecode_ready = -1;
    size->cvt_ready      = -1;
#endif

    size->ttmetrics.valid = FALSE;
    size->strike_index    = 0xFFFFFFFFUL;

    return error;
  }

  tt_size_done( FT_Size  ttsize )           /* TT_Size */
  {
    TT_Size  size = (TT_Size)ttsize;


#ifdef TT_USE_BYTECODE_INTERPRETER
    tt_size_done_bytecode( ttsize );
#endif

    size->ttmetrics.valid = FALSE;
  }

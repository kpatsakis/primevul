  Ins_SWAP( FT_Long*  args )
  {
    FT_Long  L;


    L       = args[0];
    args[0] = args[1];
    args[1] = L;
  }

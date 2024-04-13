  Ins_MUL( FT_Long*  args )
  {
    args[0] = FT_MulDiv( args[0], args[1], 64L );
  }

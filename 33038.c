  Ins_ROLL( FT_Long*  args )
  {
    FT_Long  A, B, C;


    A = args[2];
    B = args[1];
    C = args[0];

    args[2] = C;
    args[1] = A;
    args[0] = B;
  }

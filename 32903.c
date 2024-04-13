  psh_corner_orientation( FT_Pos  in_x,
                          FT_Pos  in_y,
                          FT_Pos  out_x,
                          FT_Pos  out_y )
  {
    FT_Int  result;


    /* deal with the trivial cases quickly */
    if ( in_y == 0 )
    {
      if ( in_x >= 0 )
        result = out_y;
      else
        result = -out_y;
    }
    else if ( in_x == 0 )
    {
      if ( in_y >= 0 )
        result = -out_x;
      else
        result = out_x;
    }
    else if ( out_y == 0 )
    {
      if ( out_x >= 0 )
        result = in_y;
      else
        result = -in_y;
    }
    else if ( out_x == 0 )
    {
      if ( out_y >= 0 )
        result = -in_x;
      else
        result =  in_x;
    }
    else /* general case */
    {
      long long  delta = (long long)in_x * out_y - (long long)in_y * out_x;

      if ( delta == 0 )
        result = 0;
      else
        result = 1 - 2 * ( delta < 0 );
    }

    return result;
  }

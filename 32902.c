  psh_corner_is_flat( FT_Pos  x_in,
                      FT_Pos  y_in,
                      FT_Pos  x_out,
                      FT_Pos  y_out )
  {
    FT_Pos  ax = x_in;
    FT_Pos  ay = y_in;

    FT_Pos  d_in, d_out, d_corner;


    if ( ax < 0 )
      ax = -ax;
    if ( ay < 0 )
      ay = -ay;
    d_in = ax + ay;

    ax = x_out;
    if ( ax < 0 )
      ax = -ax;
    ay = y_out;
    if ( ay < 0 )
      ay = -ay;
    d_out = ax + ay;

    ax = x_out + x_in;
    if ( ax < 0 )
      ax = -ax;
    ay = y_out + y_in;
    if ( ay < 0 )
      ay = -ay;
    d_corner = ax + ay;

    return ( d_in + d_out - d_corner ) < ( d_corner >> 4 );
  }

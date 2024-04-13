  Round_To_Half_Grid( TT_ExecContext  exc,
                      FT_F26Dot6      distance,
                      FT_F26Dot6      compensation )
  {
    FT_F26Dot6  val;

    FT_UNUSED( exc );


    if ( distance >= 0 )
    {
      val = ADD_LONG( FT_PIX_FLOOR( ADD_LONG( distance, compensation ) ),
                      32 );
      if ( val < 0 )
        val = 32;
    }
    else
    {
      val = NEG_LONG( ADD_LONG( FT_PIX_FLOOR( SUB_LONG( compensation,
                                                        distance ) ),
                                32 ) );
      if ( val > 0 )
        val = -32;
    }

    return val;
  }

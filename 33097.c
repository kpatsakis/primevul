  Round_Up_To_Grid( TT_ExecContext  exc,
                    FT_F26Dot6      distance,
                    FT_F26Dot6      compensation )
  {
    FT_F26Dot6  val;

    FT_UNUSED( exc );


    if ( distance >= 0 )
    {
      val = FT_PIX_CEIL_LONG( ADD_LONG( distance, compensation ) );
      if ( val < 0 )
        val = 0;
    }
    else
    {
      val = NEG_LONG( FT_PIX_CEIL_LONG( SUB_LONG( compensation,
                                                  distance ) ) );
      if ( val > 0 )
        val = 0;
    }

    return val;
  }

  Round_None( TT_ExecContext  exc,
              FT_F26Dot6      distance,
              FT_F26Dot6      compensation )
  {
    FT_F26Dot6  val;

    FT_UNUSED( exc );


    if ( distance >= 0 )
    {
      val = ADD_LONG( distance, compensation );
      if ( val < 0 )
        val = 0;
    }
    else
    {
      val = SUB_LONG( distance, compensation );
      if ( val > 0 )
        val = 0;
    }
    return val;
  }

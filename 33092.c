  Round_Super( TT_ExecContext  exc,
               FT_F26Dot6      distance,
               FT_F26Dot6      compensation )
  {
    FT_F26Dot6  val;


    if ( distance >= 0 )
    {
      val = ADD_LONG( distance,
                      exc->threshold - exc->phase + compensation ) &
              -exc->period;
      val += exc->phase;
      if ( val < 0 )
        val = exc->phase;
    }
    else
    {
      val = NEG_LONG( SUB_LONG( exc->threshold - exc->phase + compensation,
                                distance ) &
                        -exc->period );
      val -= exc->phase;
      if ( val > 0 )
        val = -exc->phase;
    }

    return val;
  }

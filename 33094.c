  Round_To_Double_Grid( TT_ExecContext  exc,
                        FT_F26Dot6      distance,
                        FT_F26Dot6      compensation )
  {
    FT_F26Dot6  val;

    FT_UNUSED( exc );


    if ( distance >= 0 )
    {
      val = FT_PAD_ROUND_LONG( ADD_LONG( distance, compensation ), 32 );
      if ( val < 0 )
        val = 0;
    }
    else
    {
      val = NEG_LONG( FT_PAD_ROUND_LONG( SUB_LONG( compensation, distance ),
                                         32 ) );
      if ( val > 0 )
        val = 0;
    }

    return val;
  }

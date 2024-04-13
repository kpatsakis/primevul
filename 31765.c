  End_Profile( RAS_ARGS Bool  overshoot )
  {
    Long  h;


    h = (Long)( ras.top - ras.cProfile->offset );

    if ( h < 0 )
    {
      FT_ERROR(( "End_Profile: negative height encountered\n" ));
      ras.error = FT_THROW( Neg_Height );
      return FAILURE;
    }

    if ( h > 0 )
    {
      PProfile  oldProfile;


      FT_TRACE6(( "Ending profile %p, start = %ld, height = %ld\n",
                  ras.cProfile, ras.cProfile->start, h ));

      ras.cProfile->height = h;
      if ( overshoot )
      {
        if ( ras.cProfile->flags & Flow_Up )
          ras.cProfile->flags |= Overshoot_Top;
        else
          ras.cProfile->flags |= Overshoot_Bottom;
      }

      oldProfile   = ras.cProfile;
      ras.cProfile = (PProfile)ras.top;

      ras.top += AlignProfileSize;

      ras.cProfile->height = 0;
      ras.cProfile->offset = ras.top;

      oldProfile->next = ras.cProfile;
      ras.num_Profs++;
    }

    if ( ras.top >= ras.maxBuff )
    {
      FT_TRACE1(( "overflow in End_Profile\n" ));
      ras.error = FT_THROW( Overflow );
      return FAILURE;
    }

    ras.joint = FALSE;

    return SUCCESS;
  }

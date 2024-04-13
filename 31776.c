  New_Profile( RAS_ARGS TStates  aState,
                        Bool     overshoot )
  {
    if ( !ras.fProfile )
    {
      ras.cProfile  = (PProfile)ras.top;
      ras.fProfile  = ras.cProfile;
      ras.top      += AlignProfileSize;
    }

    if ( ras.top >= ras.maxBuff )
    {
      ras.error = FT_THROW( Overflow );
      return FAILURE;
    }

    ras.cProfile->flags  = 0;
    ras.cProfile->start  = 0;
    ras.cProfile->height = 0;
    ras.cProfile->offset = ras.top;
    ras.cProfile->link   = (PProfile)0;
    ras.cProfile->next   = (PProfile)0;
    ras.cProfile->flags  = ras.dropOutControl;

    switch ( aState )
    {
    case Ascending_State:
      ras.cProfile->flags |= Flow_Up;
      if ( overshoot )
        ras.cProfile->flags |= Overshoot_Bottom;

      FT_TRACE6(( "New ascending profile = %p\n", ras.cProfile ));
      break;

    case Descending_State:
      if ( overshoot )
        ras.cProfile->flags |= Overshoot_Top;
      FT_TRACE6(( "New descending profile = %p\n", ras.cProfile ));
      break;

    default:
      FT_ERROR(( "New_Profile: invalid profile direction\n" ));
      ras.error = FT_THROW( Invalid );
      return FAILURE;
    }

    if ( !ras.gProfile )
      ras.gProfile = ras.cProfile;

    ras.state = aState;
    ras.fresh = TRUE;
    ras.joint = FALSE;

    return SUCCESS;
  }

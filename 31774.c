  Line_To( RAS_ARGS Long  x,
                    Long  y )
  {
    /* First, detect a change of direction */

    switch ( ras.state )
    {
    case Unknown_State:
      if ( y > ras.lastY )
      {
        if ( New_Profile( RAS_VARS Ascending_State,
                                   IS_BOTTOM_OVERSHOOT( ras.lastY ) ) )
          return FAILURE;
      }
      else
      {
        if ( y < ras.lastY )
          if ( New_Profile( RAS_VARS Descending_State,
                                     IS_TOP_OVERSHOOT( ras.lastY ) ) )
            return FAILURE;
      }
      break;

    case Ascending_State:
      if ( y < ras.lastY )
      {
        if ( End_Profile( RAS_VARS IS_TOP_OVERSHOOT( ras.lastY ) ) ||
             New_Profile( RAS_VARS Descending_State,
                                   IS_TOP_OVERSHOOT( ras.lastY ) ) )
          return FAILURE;
      }
      break;

    case Descending_State:
      if ( y > ras.lastY )
      {
        if ( End_Profile( RAS_VARS IS_BOTTOM_OVERSHOOT( ras.lastY ) ) ||
             New_Profile( RAS_VARS Ascending_State,
                                   IS_BOTTOM_OVERSHOOT( ras.lastY ) ) )
          return FAILURE;
      }
      break;

    default:
      ;
    }

    /* Then compute the lines */

    switch ( ras.state )
    {
    case Ascending_State:
      if ( Line_Up( RAS_VARS ras.lastX, ras.lastY,
                             x, y, ras.minY, ras.maxY ) )
        return FAILURE;
      break;

    case Descending_State:
      if ( Line_Down( RAS_VARS ras.lastX, ras.lastY,
                               x, y, ras.minY, ras.maxY ) )
        return FAILURE;
      break;

    default:
      ;
    }

    ras.lastX = x;
    ras.lastY = y;

    return SUCCESS;
  }

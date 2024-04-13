  Vertical_Sweep_Span( RAS_ARGS Short       y,
                                FT_F26Dot6  x1,
                                FT_F26Dot6  x2,
                                PProfile    left,
                                PProfile    right )
  {
    Long   e1, e2;
    Byte*  target;

    Int  dropOutControl = left->flags & 7;

    FT_UNUSED( y );
    FT_UNUSED( left );
    FT_UNUSED( right );


    /* Drop-out control */

    e1 = TRUNC( CEILING( x1 ) );

    if ( dropOutControl != 2                             &&
         x2 - x1 - ras.precision <= ras.precision_jitter )
      e2 = e1;
    else
      e2 = TRUNC( FLOOR( x2 ) );

    if ( e2 >= 0 && e1 < ras.bWidth )
    {
      int   c1, c2;
      Byte  f1, f2;


      if ( e1 < 0 )
        e1 = 0;
      if ( e2 >= ras.bWidth )
        e2 = ras.bWidth - 1;

      c1 = (Short)( e1 >> 3 );
      c2 = (Short)( e2 >> 3 );

      f1 = (Byte)  ( 0xFF >> ( e1 & 7 ) );
      f2 = (Byte) ~( 0x7F >> ( e2 & 7 ) );

      if ( ras.gray_min_x > c1 )
        ras.gray_min_x = (short)c1;
      if ( ras.gray_max_x < c2 )
        ras.gray_max_x = (short)c2;

      target = ras.bTarget + ras.traceOfs + c1;
      c2 -= c1;

      if ( c2 > 0 )
      {
        target[0] |= f1;

        /* memset() is slower than the following code on many platforms. */
        /* This is due to the fact that, in the vast majority of cases,  */
        /* the span length in bytes is relatively small.                 */
        c2--;
        while ( c2 > 0 )
        {
          *(++target) = 0xFF;
          c2--;
        }
        target[1] |= f2;
      }
      else
        *target |= ( f1 & f2 );
    }
  }

  Vertical_Sweep_Drop( RAS_ARGS Short       y,
                                FT_F26Dot6  x1,
                                FT_F26Dot6  x2,
                                PProfile    left,
                                PProfile    right )
  {
    Long   e1, e2, pxl;
    Short  c1, f1;


    /* Drop-out control */

    /*   e2            x2                    x1           e1   */
    /*                                                         */
    /*                 ^                     |                 */
    /*                 |                     |                 */
    /*   +-------------+---------------------+------------+    */
    /*                 |                     |                 */
    /*                 |                     v                 */
    /*                                                         */
    /* pixel         contour              contour       pixel  */
    /* center                                           center */

    /* drop-out mode    scan conversion rules (as defined in OpenType) */
    /* --------------------------------------------------------------- */
    /*  0                1, 2, 3                                       */
    /*  1                1, 2, 4                                       */
    /*  2                1, 2                                          */
    /*  3                same as mode 2                                */
    /*  4                1, 2, 5                                       */
    /*  5                1, 2, 6                                       */
    /*  6, 7             same as mode 2                                */

    e1  = CEILING( x1 );
    e2  = FLOOR  ( x2 );
    pxl = e1;

    if ( e1 > e2 )
    {
      Int  dropOutControl = left->flags & 7;


      if ( e1 == e2 + ras.precision )
      {
        switch ( dropOutControl )
        {
        case 0: /* simple drop-outs including stubs */
          pxl = e2;
          break;

        case 4: /* smart drop-outs including stubs */
          pxl = FLOOR( ( x1 + x2 - 1 ) / 2 + ras.precision_half );
          break;

        case 1: /* simple drop-outs excluding stubs */
        case 5: /* smart drop-outs excluding stubs  */

          /* Drop-out Control Rules #4 and #6 */

          /* The specification neither provides an exact definition */
          /* of a `stub' nor gives exact rules to exclude them.     */
          /*                                                        */
          /* Here the constraints we use to recognize a stub.       */
          /*                                                        */
          /*  upper stub:                                           */
          /*                                                        */
          /*   - P_Left and P_Right are in the same contour         */
          /*   - P_Right is the successor of P_Left in that contour */
          /*   - y is the top of P_Left and P_Right                 */
          /*                                                        */
          /*  lower stub:                                           */
          /*                                                        */
          /*   - P_Left and P_Right are in the same contour         */
          /*   - P_Left is the successor of P_Right in that contour */
          /*   - y is the bottom of P_Left                          */
          /*                                                        */
          /* We draw a stub if the following constraints are met.   */
          /*                                                        */
          /*   - for an upper or lower stub, there is top or bottom */
          /*     overshoot, respectively                            */
          /*   - the covered interval is greater or equal to a half */
          /*     pixel                                              */

          /* upper stub test */
          if ( left->next == right                &&
               left->height <= 0                  &&
               !( left->flags & Overshoot_Top   &&
                  x2 - x1 >= ras.precision_half ) )
            return;

          /* lower stub test */
          if ( right->next == left                 &&
               left->start == y                    &&
               !( left->flags & Overshoot_Bottom &&
                  x2 - x1 >= ras.precision_half  ) )
            return;

          if ( dropOutControl == 1 )
            pxl = e2;
          else
            pxl = FLOOR( ( x1 + x2 - 1 ) / 2 + ras.precision_half );
          break;

        default: /* modes 2, 3, 6, 7 */
          return;  /* no drop-out control */
        }

        /* undocumented but confirmed: If the drop-out would result in a  */
        /* pixel outside of the bounding box, use the pixel inside of the */
        /* bounding box instead                                           */
        if ( pxl < 0 )
          pxl = e1;
        else if ( TRUNC( pxl ) >= ras.bWidth )
          pxl = e2;

        /* check that the other pixel isn't set */
        e1 = pxl == e1 ? e2 : e1;

        e1 = TRUNC( e1 );

        c1 = (Short)( e1 >> 3 );
        f1 = (Short)( e1 &  7 );

        if ( e1 >= 0 && e1 < ras.bWidth                      &&
             ras.bTarget[ras.traceOfs + c1] & ( 0x80 >> f1 ) )
          return;
      }
      else
        return;
    }

    e1 = TRUNC( pxl );

    if ( e1 >= 0 && e1 < ras.bWidth )
    {
      c1 = (Short)( e1 >> 3 );
      f1 = (Short)( e1 & 7 );

      if ( ras.gray_min_x > c1 )
        ras.gray_min_x = c1;
      if ( ras.gray_max_x < c1 )
        ras.gray_max_x = c1;

      ras.bTarget[ras.traceOfs + c1] |= (char)( 0x80 >> f1 );
    }
  }

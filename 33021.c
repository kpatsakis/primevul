  Ins_MIAP( TT_ExecContext  exc,
            FT_Long*        args )
  {
    FT_ULong    cvtEntry;
    FT_UShort   point;
    FT_F26Dot6  distance;
    FT_F26Dot6  org_dist;
    FT_F26Dot6  control_value_cutin;


    control_value_cutin = exc->GS.control_value_cutin;
    cvtEntry            = (FT_ULong)args[1];
    point               = (FT_UShort)args[0];

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    if ( SUBPIXEL_HINTING_INFINALITY                        &&
         exc->ignore_x_mode                                 &&
         exc->GS.freeVector.x != 0                          &&
         exc->GS.freeVector.y == 0                          &&
         !( exc->sph_tweak_flags & SPH_TWEAK_NORMAL_ROUND ) )
      control_value_cutin = 0;
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

    if ( BOUNDS( point,     exc->zp0.n_points ) ||
         BOUNDSL( cvtEntry, exc->cvtSize )      )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      goto Fail;
    }

    /* UNDOCUMENTED!                                                      */
    /*                                                                    */
    /* The behaviour of an MIAP instruction is quite different when used  */
    /* in the twilight zone.                                              */
    /*                                                                    */
    /* First, no control value cut-in test is performed as it would fail  */
    /* anyway.  Second, the original point, i.e. (org_x,org_y) of         */
    /* zp0.point, is set to the absolute, unrounded distance found in the */
    /* CVT.                                                               */
    /*                                                                    */
    /* This is used in the CVT programs of the Microsoft fonts Arial,     */
    /* Times, etc., in order to re-adjust some key font heights.  It      */
    /* allows the use of the IP instruction in the twilight zone, which   */
    /* otherwise would be invalid according to the specification.         */
    /*                                                                    */
    /* We implement it with a special sequence for the twilight zone.     */
    /* This is a bad hack, but it seems to work.                          */
    /*                                                                    */
    /* Confirmed by Greg Hitchcock.                                       */

    distance = exc->func_read_cvt( exc, cvtEntry );

    if ( exc->GS.gep0 == 0 )   /* If in twilight zone */
    {
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
      /* Only adjust if not in sph_compatibility_mode or ignore_x_mode. */
      /* Determined via experimentation and may be incorrect...         */
      if ( !( SUBPIXEL_HINTING_INFINALITY           &&
              ( exc->ignore_x_mode                &&
                exc->face->sph_compatibility_mode ) ) )
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */
        exc->zp0.org[point].x = TT_MulFix14( distance,
                                             exc->GS.freeVector.x );
      exc->zp0.org[point].y = TT_MulFix14( distance,
                                           exc->GS.freeVector.y ),
      exc->zp0.cur[point]   = exc->zp0.org[point];
    }
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    if ( SUBPIXEL_HINTING_INFINALITY                    &&
         exc->ignore_x_mode                             &&
         ( exc->sph_tweak_flags & SPH_TWEAK_MIAP_HACK ) &&
         distance > 0                                   &&
         exc->GS.freeVector.y != 0                      )
      distance = 0;
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

    org_dist = FAST_PROJECT( &exc->zp0.cur[point] );

    if ( ( exc->opcode & 1 ) != 0 )   /* rounding and control cut-in flag */
    {
      if ( FT_ABS( distance - org_dist ) > control_value_cutin )
        distance = org_dist;

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
      if ( SUBPIXEL_HINTING_INFINALITY &&
           exc->ignore_x_mode          &&
           exc->GS.freeVector.x != 0   )
        distance = Round_None( exc,
                               distance,
                               exc->tt_metrics.compensations[0] );
      else
#endif
        distance = exc->func_round( exc,
                                    distance,
                                    exc->tt_metrics.compensations[0] );
    }

    exc->func_move( exc, &exc->zp0, point, SUB_LONG( distance, org_dist ) );

  Fail:
    exc->GS.rp0 = point;
    exc->GS.rp1 = point;
  }

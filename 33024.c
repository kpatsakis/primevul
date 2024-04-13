  Ins_MIRP( TT_ExecContext  exc,
            FT_Long*        args )
  {
    FT_UShort   point;
    FT_ULong    cvtEntry;

    FT_F26Dot6  cvt_dist,
                distance,
                cur_dist,
                org_dist,
                control_value_cutin,
                minimum_distance;
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    FT_Int      B1           = 0; /* pacify compiler */
    FT_Int      B2           = 0;
    FT_Bool     reverse_move = FALSE;
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */


    minimum_distance    = exc->GS.minimum_distance;
    control_value_cutin = exc->GS.control_value_cutin;
    point               = (FT_UShort)args[0];
    cvtEntry            = (FT_ULong)( args[1] + 1 );

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    if ( SUBPIXEL_HINTING_INFINALITY                        &&
         exc->ignore_x_mode                                 &&
         exc->GS.freeVector.x != 0                          &&
         !( exc->sph_tweak_flags & SPH_TWEAK_NORMAL_ROUND ) )
      control_value_cutin = minimum_distance = 0;
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

    /* XXX: UNDOCUMENTED! cvt[-1] = 0 always */

    if ( BOUNDS( point,       exc->zp1.n_points ) ||
         BOUNDSL( cvtEntry,   exc->cvtSize + 1 )  ||
         BOUNDS( exc->GS.rp0, exc->zp0.n_points ) )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      goto Fail;
    }

    if ( !cvtEntry )
      cvt_dist = 0;
    else
      cvt_dist = exc->func_read_cvt( exc, cvtEntry - 1 );

    /* single width test */

    if ( FT_ABS( cvt_dist - exc->GS.single_width_value ) <
         exc->GS.single_width_cutin )
    {
      if ( cvt_dist >= 0 )
        cvt_dist =  exc->GS.single_width_value;
      else
        cvt_dist = -exc->GS.single_width_value;
    }

    /* UNDOCUMENTED!  The MS rasterizer does that with */
    /* twilight points (confirmed by Greg Hitchcock)   */
    if ( exc->GS.gep1 == 0 )
    {
      exc->zp1.org[point].x = exc->zp0.org[exc->GS.rp0].x +
                              TT_MulFix14( cvt_dist,
                                           exc->GS.freeVector.x );
      exc->zp1.org[point].y = exc->zp0.org[exc->GS.rp0].y +
                              TT_MulFix14( cvt_dist,
                                           exc->GS.freeVector.y );
      exc->zp1.cur[point]   = exc->zp1.org[point];
    }

    org_dist = DUALPROJ( &exc->zp1.org[point], &exc->zp0.org[exc->GS.rp0] );
    cur_dist = PROJECT ( &exc->zp1.cur[point], &exc->zp0.cur[exc->GS.rp0] );

    /* auto-flip test */

    if ( exc->GS.auto_flip )
    {
      if ( ( org_dist ^ cvt_dist ) < 0 )
        cvt_dist = -cvt_dist;
    }

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    if ( SUBPIXEL_HINTING_INFINALITY                               &&
         exc->ignore_x_mode                                        &&
         exc->GS.freeVector.y != 0                                 &&
         ( exc->sph_tweak_flags & SPH_TWEAK_TIMES_NEW_ROMAN_HACK ) )
    {
      if ( cur_dist < -64 )
        cvt_dist -= 16;
      else if ( cur_dist > 64 && cur_dist < 84 )
        cvt_dist += 32;
    }
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

    /* control value cut-in and round */

    if ( ( exc->opcode & 4 ) != 0 )
    {
      /* XXX: UNDOCUMENTED!  Only perform cut-in test when both points */
      /*      refer to the same zone.                                  */

      if ( exc->GS.gep0 == exc->GS.gep1 )
      {
        /* XXX: According to Greg Hitchcock, the following wording is */
        /*      the right one:                                        */
        /*                                                            */
        /*        When the absolute difference between the value in   */
        /*        the table [CVT] and the measurement directly from   */
        /*        the outline is _greater_ than the cut_in value, the */
        /*        outline measurement is used.                        */
        /*                                                            */
        /*      This is from `instgly.doc'.  The description in       */
        /*      `ttinst2.doc', version 1.66, is thus incorrect since  */
        /*      it implies `>=' instead of `>'.                       */

        if ( FT_ABS( cvt_dist - org_dist ) > control_value_cutin )
          cvt_dist = org_dist;
      }

      distance = exc->func_round(
                   exc,
                   cvt_dist,
                   exc->tt_metrics.compensations[exc->opcode & 3] );
    }
    else
    {

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
      /* do cvt cut-in always in MIRP for sph */
      if ( SUBPIXEL_HINTING_INFINALITY  &&
           exc->ignore_x_mode           &&
           exc->GS.gep0 == exc->GS.gep1 )
      {
        if ( FT_ABS( cvt_dist - org_dist ) > control_value_cutin )
          cvt_dist = org_dist;
      }
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

      distance = Round_None(
                   exc,
                   cvt_dist,
                   exc->tt_metrics.compensations[exc->opcode & 3] );
    }

    /* minimum distance test */

    if ( ( exc->opcode & 8 ) != 0 )
    {
      if ( org_dist >= 0 )
      {
        if ( distance < minimum_distance )
          distance = minimum_distance;
      }
      else
      {
        if ( distance > NEG_LONG( minimum_distance ) )
          distance = NEG_LONG( minimum_distance );
      }
    }

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    if ( SUBPIXEL_HINTING_INFINALITY )
    {
      B1 = exc->zp1.cur[point].y;

      /* Round moves if necessary */
      if ( exc->ignore_x_mode                                          &&
           exc->GS.freeVector.y != 0                                   &&
           ( exc->sph_tweak_flags & SPH_TWEAK_ROUND_NONPIXEL_Y_MOVES ) )
        distance = FT_PIX_ROUND( B1 + distance - cur_dist ) - B1 + cur_dist;

      if ( exc->ignore_x_mode                                      &&
           exc->GS.freeVector.y != 0                               &&
           ( exc->opcode & 16 ) == 0                               &&
           ( exc->opcode & 8 ) == 0                                &&
           ( exc->sph_tweak_flags & SPH_TWEAK_COURIER_NEW_2_HACK ) )
        distance += 64;
    }
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

    exc->func_move( exc,
                    &exc->zp1,
                    point,
                    SUB_LONG( distance, cur_dist ) );

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    if ( SUBPIXEL_HINTING_INFINALITY )
    {
      B2 = exc->zp1.cur[point].y;

      /* Reverse move if necessary */
      if ( exc->ignore_x_mode )
      {
        if ( exc->face->sph_compatibility_mode &&
             exc->GS.freeVector.y != 0         &&
             ( B1 & 63 ) == 0                  &&
             ( B2 & 63 ) != 0                  )
          reverse_move = TRUE;

        if ( ( exc->sph_tweak_flags & SPH_TWEAK_SKIP_NONPIXEL_Y_MOVES ) &&
             exc->GS.freeVector.y != 0                                  &&
             ( B2 & 63 ) != 0                                           &&
             ( B1 & 63 ) != 0                                           )
          reverse_move = TRUE;
      }

      if ( reverse_move )
        exc->func_move( exc,
                        &exc->zp1,
                        point,
                        SUB_LONG( cur_dist, distance ) );
    }

#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

  Fail:
    exc->GS.rp1 = exc->GS.rp0;

    if ( ( exc->opcode & 16 ) != 0 )
      exc->GS.rp0 = point;

    exc->GS.rp2 = point;
  }

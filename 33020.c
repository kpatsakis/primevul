  Ins_MDRP( TT_ExecContext  exc,
            FT_Long*        args )
  {
    FT_UShort   point = 0;
    FT_F26Dot6  org_dist, distance, minimum_distance;


    minimum_distance = exc->GS.minimum_distance;

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    if ( SUBPIXEL_HINTING_INFINALITY                        &&
         exc->ignore_x_mode                                 &&
         exc->GS.freeVector.x != 0                          &&
         !( exc->sph_tweak_flags & SPH_TWEAK_NORMAL_ROUND ) )
      minimum_distance = 0;
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

    point = (FT_UShort)args[0];

    if ( BOUNDS( point,       exc->zp1.n_points ) ||
         BOUNDS( exc->GS.rp0, exc->zp0.n_points ) )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      goto Fail;
    }

    /* XXX: Is there some undocumented feature while in the */
    /*      twilight zone?                                  */

    /* XXX: UNDOCUMENTED: twilight zone special case */

    if ( exc->GS.gep0 == 0 || exc->GS.gep1 == 0 )
    {
      FT_Vector*  vec1 = &exc->zp1.org[point];
      FT_Vector*  vec2 = &exc->zp0.org[exc->GS.rp0];


      org_dist = DUALPROJ( vec1, vec2 );
    }
    else
    {
      FT_Vector*  vec1 = &exc->zp1.orus[point];
      FT_Vector*  vec2 = &exc->zp0.orus[exc->GS.rp0];


      if ( exc->metrics.x_scale == exc->metrics.y_scale )
      {
        /* this should be faster */
        org_dist = DUALPROJ( vec1, vec2 );
        org_dist = FT_MulFix( org_dist, exc->metrics.x_scale );
      }
      else
      {
        FT_Vector  vec;


        vec.x = FT_MulFix( SUB_LONG( vec1->x, vec2->x ),
                           exc->metrics.x_scale );
        vec.y = FT_MulFix( SUB_LONG( vec1->y, vec2->y ),
                           exc->metrics.y_scale );

        org_dist = FAST_DUALPROJ( &vec );
      }
    }

    /* single width cut-in test */

    /* |org_dist - single_width_value| < single_width_cutin */
    if ( exc->GS.single_width_cutin > 0          &&
         org_dist < exc->GS.single_width_value +
                      exc->GS.single_width_cutin &&
         org_dist > exc->GS.single_width_value -
                      exc->GS.single_width_cutin )
    {
      if ( org_dist >= 0 )
        org_dist = exc->GS.single_width_value;
      else
        org_dist = -exc->GS.single_width_value;
    }

    /* round flag */

    if ( ( exc->opcode & 4 ) != 0 )
    {
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
      if ( SUBPIXEL_HINTING_INFINALITY &&
           exc->ignore_x_mode          &&
           exc->GS.freeVector.x != 0   )
        distance = Round_None(
                     exc,
                     org_dist,
                     exc->tt_metrics.compensations[exc->opcode & 3] );
      else
#endif
        distance = exc->func_round(
                     exc,
                     org_dist,
                     exc->tt_metrics.compensations[exc->opcode & 3] );
    }
    else
      distance = Round_None(
                   exc,
                   org_dist,
                   exc->tt_metrics.compensations[exc->opcode & 3] );

    /* minimum distance flag */

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

    /* now move the point */

    org_dist = PROJECT( exc->zp1.cur + point, exc->zp0.cur + exc->GS.rp0 );

    exc->func_move( exc, &exc->zp1, point, SUB_LONG( distance, org_dist ) );

  Fail:
    exc->GS.rp1 = exc->GS.rp0;
    exc->GS.rp2 = point;

    if ( ( exc->opcode & 16 ) != 0 )
      exc->GS.rp0 = point;
  }

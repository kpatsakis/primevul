  Ins_MSIRP( TT_ExecContext  exc,
             FT_Long*        args )
  {
    FT_UShort   point = 0;
    FT_F26Dot6  distance;
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    FT_F26Dot6  control_value_cutin = 0;


    if ( SUBPIXEL_HINTING_INFINALITY )
    {
      control_value_cutin = exc->GS.control_value_cutin;

      if ( exc->ignore_x_mode                                 &&
           exc->GS.freeVector.x != 0                          &&
           !( exc->sph_tweak_flags & SPH_TWEAK_NORMAL_ROUND ) )
        control_value_cutin = 0;
    }
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

    point = (FT_UShort)args[0];

    if ( BOUNDS( point,       exc->zp1.n_points ) ||
         BOUNDS( exc->GS.rp0, exc->zp0.n_points ) )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      return;
    }

    /* UNDOCUMENTED!  The MS rasterizer does that with */
    /* twilight points (confirmed by Greg Hitchcock)   */
    if ( exc->GS.gep1 == 0 )
    {
      exc->zp1.org[point] = exc->zp0.org[exc->GS.rp0];
      exc->func_move_orig( exc, &exc->zp1, point, args[1] );
      exc->zp1.cur[point] = exc->zp1.org[point];
    }

    distance = PROJECT( exc->zp1.cur + point, exc->zp0.cur + exc->GS.rp0 );

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    /* subpixel hinting - make MSIRP respect CVT cut-in; */
    if ( SUBPIXEL_HINTING_INFINALITY                                    &&
         exc->ignore_x_mode                                             &&
         exc->GS.freeVector.x != 0                                      &&
         FT_ABS( SUB_LONG( distance, args[1] ) ) >= control_value_cutin )
      distance = args[1];
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

    exc->func_move( exc,
                    &exc->zp1,
                    point,
                    SUB_LONG( args[1], distance ) );

    exc->GS.rp1 = exc->GS.rp0;
    exc->GS.rp2 = point;

    if ( ( exc->opcode & 1 ) != 0 )
      exc->GS.rp0 = point;
  }

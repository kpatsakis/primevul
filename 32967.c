  Direct_Move( TT_ExecContext  exc,
               TT_GlyphZone    zone,
               FT_UShort       point,
               FT_F26Dot6      distance )
  {
    FT_F26Dot6  v;


    v = exc->GS.freeVector.x;

    if ( v != 0 )
    {
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
      if ( SUBPIXEL_HINTING_INFINALITY                            &&
           ( !exc->ignore_x_mode                                ||
             ( exc->sph_tweak_flags & SPH_TWEAK_ALLOW_X_DMOVE ) ) )
        zone->cur[point].x = ADD_LONG( zone->cur[point].x,
                                       FT_MulDiv( distance,
                                                  v,
                                                  exc->F_dot_P ) );
      else
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_MINIMAL
      /* Exception to the post-IUP curfew: Allow the x component of */
      /* diagonal moves, but only post-IUP.  DejaVu tries to adjust */
      /* diagonal stems like on `Z' and `z' post-IUP.               */
      if ( SUBPIXEL_HINTING_MINIMAL && !exc->backward_compatibility )
        zone->cur[point].x = ADD_LONG( zone->cur[point].x,
                                       FT_MulDiv( distance,
                                                  v,
                                                  exc->F_dot_P ) );
      else
#endif

      if ( NO_SUBPIXEL_HINTING )
        zone->cur[point].x = ADD_LONG( zone->cur[point].x,
                                       FT_MulDiv( distance,
                                                  v,
                                                  exc->F_dot_P ) );

      zone->tags[point] |= FT_CURVE_TAG_TOUCH_X;
    }

    v = exc->GS.freeVector.y;

    if ( v != 0 )
    {
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_MINIMAL
      if ( !( SUBPIXEL_HINTING_MINIMAL    &&
              exc->backward_compatibility &&
              exc->iupx_called            &&
              exc->iupy_called            ) )
#endif
        zone->cur[point].y = ADD_LONG( zone->cur[point].y,
                                       FT_MulDiv( distance,
                                                  v,
                                                  exc->F_dot_P ) );

      zone->tags[point] |= FT_CURVE_TAG_TOUCH_Y;
    }
  }

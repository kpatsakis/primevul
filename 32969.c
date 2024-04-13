  Direct_Move_X( TT_ExecContext  exc,
                 TT_GlyphZone    zone,
                 FT_UShort       point,
                 FT_F26Dot6      distance )
  {
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    if ( SUBPIXEL_HINTING_INFINALITY && !exc->ignore_x_mode )
      zone->cur[point].x = ADD_LONG( zone->cur[point].x, distance );
    else
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_MINIMAL
    if ( SUBPIXEL_HINTING_MINIMAL && !exc->backward_compatibility )
      zone->cur[point].x = ADD_LONG( zone->cur[point].x, distance );
    else
#endif

    if ( NO_SUBPIXEL_HINTING )
      zone->cur[point].x = ADD_LONG( zone->cur[point].x, distance );

    zone->tags[point]  |= FT_CURVE_TAG_TOUCH_X;
  }

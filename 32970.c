  Direct_Move_Y( TT_ExecContext  exc,
                 TT_GlyphZone    zone,
                 FT_UShort       point,
                 FT_F26Dot6      distance )
  {
    FT_UNUSED( exc );

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_MINIMAL
    if ( !( SUBPIXEL_HINTING_MINIMAL             &&
            exc->backward_compatibility          &&
            exc->iupx_called && exc->iupy_called ) )
#endif
      zone->cur[point].y = ADD_LONG( zone->cur[point].y, distance );

    zone->tags[point] |= FT_CURVE_TAG_TOUCH_Y;
  }

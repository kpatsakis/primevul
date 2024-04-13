  Direct_Move_Orig_X( TT_ExecContext  exc,
                      TT_GlyphZone    zone,
                      FT_UShort       point,
                      FT_F26Dot6      distance )
  {
    FT_UNUSED( exc );

    zone->org[point].x = ADD_LONG( zone->org[point].x, distance );
  }

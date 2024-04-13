  Ins_SHC( TT_ExecContext  exc,
           FT_Long*        args )
  {
    TT_GlyphZoneRec  zp;
    FT_UShort        refp;
    FT_F26Dot6       dx, dy;

    FT_Short         contour, bounds;
    FT_UShort        start, limit, i;


    contour = (FT_Short)args[0];
    bounds  = ( exc->GS.gep2 == 0 ) ? 1 : exc->zp2.n_contours;

    if ( BOUNDS( contour, bounds ) )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      return;
    }

    if ( Compute_Point_Displacement( exc, &dx, &dy, &zp, &refp ) )
      return;

    if ( contour == 0 )
      start = 0;
    else
      start = (FT_UShort)( exc->zp2.contours[contour - 1] + 1 -
                           exc->zp2.first_point );

    /* we use the number of points if in the twilight zone */
    if ( exc->GS.gep2 == 0 )
      limit = exc->zp2.n_points;
    else
      limit = (FT_UShort)( exc->zp2.contours[contour] -
                           exc->zp2.first_point + 1 );

    for ( i = start; i < limit; i++ )
    {
      if ( zp.cur != exc->zp2.cur || refp != i )
        Move_Zp2_Point( exc, i, dx, dy, TRUE );
    }
  }

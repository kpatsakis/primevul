  Ins_SHZ( INS_ARG )
  {
    TT_GlyphZoneRec zp;
    FT_UShort       refp;
    FT_F26Dot6      dx,
                    dy;

    FT_UShort       last_point, i;


    if ( BOUNDS( args[0], 2 ) )
    {
      if ( CUR.pedantic_hinting )
        CUR.error = TT_Err_Invalid_Reference;
      return;
    }

    if ( COMPUTE_Point_Displacement( &dx, &dy, &zp, &refp ) )
      return;

    /* XXX: UNDOCUMENTED! SHZ doesn't move the phantom points.  */
    /*      Twilight zone has no contours, so use `n_points'.   */
    /*      Normal zone's `n_points' includes phantoms, so must */
    /*      use end of last contour.                            */
    if ( CUR.GS.gep2 == 0 && CUR.zp2.n_points > 0 )
      last_point = (FT_UShort)( CUR.zp2.n_points - 1 );
    else if ( CUR.GS.gep2 == 1 && CUR.zp2.n_contours > 0 )
      last_point = (FT_UShort)( CUR.zp2.contours[CUR.zp2.n_contours - 1] );
    else
      last_point = 0;

    /* XXX: UNDOCUMENTED! SHZ doesn't touch the points */
    for ( i = 0; i <= last_point; i++ )
    {
      if ( zp.cur != CUR.zp2.cur || refp != i )
        MOVE_Zp2_Point( i, dx, dy, FALSE );
    }
  }

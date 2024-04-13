  Ins_SHP( TT_ExecContext  exc )
  {
    TT_GlyphZoneRec  zp;
    FT_UShort        refp;

    FT_F26Dot6       dx, dy;
    FT_UShort        point;


    if ( exc->top < exc->GS.loop )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      goto Fail;
    }

    if ( Compute_Point_Displacement( exc, &dx, &dy, &zp, &refp ) )
      return;

    while ( exc->GS.loop > 0 )
    {
      exc->args--;
      point = (FT_UShort)exc->stack[exc->args];

      if ( BOUNDS( point, exc->zp2.n_points ) )
      {
        if ( exc->pedantic_hinting )
        {
          exc->error = FT_THROW( Invalid_Reference );
          return;
        }
      }
      else
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
      /* doesn't follow Cleartype spec but produces better result */
      if ( SUBPIXEL_HINTING_INFINALITY && exc->ignore_x_mode )
        Move_Zp2_Point( exc, point, 0, dy, TRUE );
      else
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */
        Move_Zp2_Point( exc, point, dx, dy, TRUE );

      exc->GS.loop--;
    }

  Fail:
    exc->GS.loop = 1;
    exc->new_top = exc->args;
  }

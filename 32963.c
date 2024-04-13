  Compute_Point_Displacement( TT_ExecContext  exc,
                              FT_F26Dot6*     x,
                              FT_F26Dot6*     y,
                              TT_GlyphZone    zone,
                              FT_UShort*      refp )
  {
    TT_GlyphZoneRec  zp;
    FT_UShort        p;
    FT_F26Dot6       d;


    if ( exc->opcode & 1 )
    {
      zp = exc->zp0;
      p  = exc->GS.rp1;
    }
    else
    {
      zp = exc->zp1;
      p  = exc->GS.rp2;
    }

    if ( BOUNDS( p, zp.n_points ) )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      *refp = 0;
      return FAILURE;
    }

    *zone = zp;
    *refp = p;

    d = PROJECT( zp.cur + p, zp.org + p );

    *x = FT_MulDiv( d, (FT_Long)exc->GS.freeVector.x, exc->F_dot_P );
    *y = FT_MulDiv( d, (FT_Long)exc->GS.freeVector.y, exc->F_dot_P );

    return SUCCESS;
  }

  Ins_ALIGNPTS( TT_ExecContext  exc,
                FT_Long*        args )
  {
    FT_UShort   p1, p2;
    FT_F26Dot6  distance;


    p1 = (FT_UShort)args[0];
    p2 = (FT_UShort)args[1];

    if ( BOUNDS( p1, exc->zp1.n_points ) ||
         BOUNDS( p2, exc->zp0.n_points ) )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      return;
    }

    distance = PROJECT( exc->zp0.cur + p2, exc->zp1.cur + p1 ) / 2;

    exc->func_move( exc, &exc->zp1, p1, distance );
    exc->func_move( exc, &exc->zp0, p2, NEG_LONG( distance ) );
  }

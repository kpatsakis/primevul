  Ins_GC( TT_ExecContext  exc,
          FT_Long*        args )
  {
    FT_ULong    L;
    FT_F26Dot6  R;


    L = (FT_ULong)args[0];

    if ( BOUNDSL( L, exc->zp2.n_points ) )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      R = 0;
    }
    else
    {
      if ( exc->opcode & 1 )
        R = FAST_DUALPROJ( &exc->zp2.org[L] );
      else
        R = FAST_PROJECT( &exc->zp2.cur[L] );
    }

    args[0] = R;
  }

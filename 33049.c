  Ins_SCFS( TT_ExecContext  exc,
            FT_Long*        args )
  {
    FT_Long    K;
    FT_UShort  L;


    L = (FT_UShort)args[0];

    if ( BOUNDS( L, exc->zp2.n_points ) )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      return;
    }

    K = FAST_PROJECT( &exc->zp2.cur[L] );

    exc->func_move( exc, &exc->zp2, L, SUB_LONG( args[1], K ) );

    /* UNDOCUMENTED!  The MS rasterizer does that with */
    /* twilight points (confirmed by Greg Hitchcock)   */
    if ( exc->GS.gep2 == 0 )
      exc->zp2.org[L] = exc->zp2.cur[L];
  }

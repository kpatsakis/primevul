  Ins_FLIPRGOFF( TT_ExecContext  exc,
                 FT_Long*        args )
  {
    FT_UShort  I, K, L;


#ifdef TT_SUPPORT_SUBPIXEL_HINTING_MINIMAL
    /* See `ttinterp.h' for details on backward compatibility mode. */
    if ( SUBPIXEL_HINTING_MINIMAL    &&
         exc->backward_compatibility &&
         exc->iupx_called            &&
         exc->iupy_called            )
      return;
#endif

    K = (FT_UShort)args[1];
    L = (FT_UShort)args[0];

    if ( BOUNDS( K, exc->pts.n_points ) ||
         BOUNDS( L, exc->pts.n_points ) )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      return;
    }

    for ( I = L; I <= K; I++ )
      exc->pts.tags[I] &= ~FT_CURVE_TAG_ON;
  }

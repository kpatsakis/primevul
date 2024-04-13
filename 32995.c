  Ins_FLIPPT( TT_ExecContext  exc )
  {
    FT_UShort  point;


#ifdef TT_SUPPORT_SUBPIXEL_HINTING_MINIMAL
    /* See `ttinterp.h' for details on backward compatibility mode. */
    if ( SUBPIXEL_HINTING_MINIMAL    &&
         exc->backward_compatibility &&
         exc->iupx_called            &&
         exc->iupy_called            )
      goto Fail;
#endif

    if ( exc->top < exc->GS.loop )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Too_Few_Arguments );
      goto Fail;
    }

    while ( exc->GS.loop > 0 )
    {
      exc->args--;

      point = (FT_UShort)exc->stack[exc->args];

      if ( BOUNDS( point, exc->pts.n_points ) )
      {
        if ( exc->pedantic_hinting )
        {
          exc->error = FT_THROW( Invalid_Reference );
          return;
        }
      }
      else
        exc->pts.tags[point] ^= FT_CURVE_TAG_ON;

      exc->GS.loop--;
    }

  Fail:
    exc->GS.loop = 1;
    exc->new_top = exc->args;
  }

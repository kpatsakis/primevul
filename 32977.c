  Ins_ALIGNRP( TT_ExecContext  exc )
  {
    FT_UShort   point;
    FT_F26Dot6  distance;


#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    if ( SUBPIXEL_HINTING_INFINALITY                               &&
         exc->ignore_x_mode                                        &&
         exc->iup_called                                           &&
         ( exc->sph_tweak_flags & SPH_TWEAK_NO_ALIGNRP_AFTER_IUP ) )
    {
      exc->error = FT_THROW( Invalid_Reference );
      goto Fail;
    }
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

    if ( exc->top < exc->GS.loop                  ||
         BOUNDS( exc->GS.rp0, exc->zp0.n_points ) )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      goto Fail;
    }

    while ( exc->GS.loop > 0 )
    {
      exc->args--;

      point = (FT_UShort)exc->stack[exc->args];

      if ( BOUNDS( point, exc->zp1.n_points ) )
      {
        if ( exc->pedantic_hinting )
        {
          exc->error = FT_THROW( Invalid_Reference );
          return;
        }
      }
      else
      {
        distance = PROJECT( exc->zp1.cur + point,
                            exc->zp0.cur + exc->GS.rp0 );

        exc->func_move( exc, &exc->zp1, point, NEG_LONG( distance ) );
      }

      exc->GS.loop--;
    }

  Fail:
    exc->GS.loop = 1;
    exc->new_top = exc->args;
  }

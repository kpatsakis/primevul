  Ins_DELTAP( TT_ExecContext  exc,
              FT_Long*        args )
  {
    FT_ULong   nump, k;
    FT_UShort  A;
    FT_ULong   C, P;
    FT_Long    B;
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    FT_UShort  B1, B2;


    if ( SUBPIXEL_HINTING_INFINALITY                              &&
         exc->ignore_x_mode                                       &&
         exc->iup_called                                          &&
         ( exc->sph_tweak_flags & SPH_TWEAK_NO_DELTAP_AFTER_IUP ) )
      goto Fail;
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

    P    = (FT_ULong)exc->func_cur_ppem( exc );
    nump = (FT_ULong)args[0];   /* some points theoretically may occur more
                                   than once, thus UShort isn't enough */

    for ( k = 1; k <= nump; k++ )
    {
      if ( exc->args < 2 )
      {
        if ( exc->pedantic_hinting )
          exc->error = FT_THROW( Too_Few_Arguments );
        exc->args = 0;
        goto Fail;
      }

      exc->args -= 2;

      A = (FT_UShort)exc->stack[exc->args + 1];
      B = exc->stack[exc->args];

      /* XXX: Because some popular fonts contain some invalid DeltaP */
      /*      instructions, we simply ignore them when the stacked   */
      /*      point reference is off limit, rather than returning an */
      /*      error.  As a delta instruction doesn't change a glyph  */
      /*      in great ways, this shouldn't be a problem.            */

      if ( !BOUNDS( A, exc->zp0.n_points ) )
      {
        C = ( (FT_ULong)B & 0xF0 ) >> 4;

        switch ( exc->opcode )
        {
        case 0x5D:
          break;

        case 0x71:
          C += 16;
          break;

        case 0x72:
          C += 32;
          break;
        }

        C += exc->GS.delta_base;

        if ( P == C )
        {
          B = ( (FT_ULong)B & 0xF ) - 8;
          if ( B >= 0 )
            B++;
          B *= 1L << ( 6 - exc->GS.delta_shift );

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY

          if ( SUBPIXEL_HINTING_INFINALITY )
          {
            /*
             *  Allow delta move if
             *
             *  - not using ignore_x_mode rendering,
             *  - glyph is specifically set to allow it, or
             *  - glyph is composite and freedom vector is not in subpixel
             *    direction.
             */
            if ( !exc->ignore_x_mode                                   ||
                 ( exc->sph_tweak_flags & SPH_TWEAK_ALWAYS_DO_DELTAP ) ||
                 ( exc->is_composite && exc->GS.freeVector.y != 0 )    )
              exc->func_move( exc, &exc->zp0, A, B );

            /* Otherwise, apply subpixel hinting and compatibility mode */
            /* rules, always skipping deltas in subpixel direction.     */
            else if ( exc->ignore_x_mode && exc->GS.freeVector.y != 0 )
            {
              /* save the y value of the point now; compare after move */
              B1 = (FT_UShort)exc->zp0.cur[A].y;

              /* Standard subpixel hinting: Allow y move for y-touched */
              /* points.  This messes up DejaVu ...                    */
              if ( !exc->face->sph_compatibility_mode          &&
                   ( exc->zp0.tags[A] & FT_CURVE_TAG_TOUCH_Y ) )
                exc->func_move( exc, &exc->zp0, A, B );

              /* compatibility mode */
              else if ( exc->face->sph_compatibility_mode                        &&
                        !( exc->sph_tweak_flags & SPH_TWEAK_ALWAYS_SKIP_DELTAP ) )
              {
                if ( exc->sph_tweak_flags & SPH_TWEAK_ROUND_NONPIXEL_Y_MOVES )
                  B = FT_PIX_ROUND( B1 + B ) - B1;

                /* Allow delta move if using sph_compatibility_mode,   */
                /* IUP has not been called, and point is touched on Y. */
                if ( !exc->iup_called                            &&
                     ( exc->zp0.tags[A] & FT_CURVE_TAG_TOUCH_Y ) )
                  exc->func_move( exc, &exc->zp0, A, B );
              }

              B2 = (FT_UShort)exc->zp0.cur[A].y;

              /* Reverse this move if it results in a disallowed move */
              if ( exc->GS.freeVector.y != 0                          &&
                   ( ( exc->face->sph_compatibility_mode          &&
                       ( B1 & 63 ) == 0                           &&
                       ( B2 & 63 ) != 0                           ) ||
                     ( ( exc->sph_tweak_flags                   &
                         SPH_TWEAK_SKIP_NONPIXEL_Y_MOVES_DELTAP ) &&
                       ( B1 & 63 ) != 0                           &&
                       ( B2 & 63 ) != 0                           ) ) )
                exc->func_move( exc, &exc->zp0, A, NEG_LONG( B ) );
            }
          }
          else
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

          {

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_MINIMAL
            /* See `ttinterp.h' for details on backward compatibility */
            /* mode.                                                  */
            if ( SUBPIXEL_HINTING_MINIMAL    &&
                 exc->backward_compatibility )
            {
              if ( !( exc->iupx_called && exc->iupy_called )              &&
                   ( ( exc->is_composite && exc->GS.freeVector.y != 0 ) ||
                     ( exc->zp0.tags[A] & FT_CURVE_TAG_TOUCH_Y )        ) )
                exc->func_move( exc, &exc->zp0, A, B );
            }
            else
#endif
              exc->func_move( exc, &exc->zp0, A, B );
          }
        }
      }
      else
        if ( exc->pedantic_hinting )
          exc->error = FT_THROW( Invalid_Reference );
    }

  Fail:
    exc->new_top = exc->args;
  }

  Ins_MD( TT_ExecContext  exc,
          FT_Long*        args )
  {
    FT_UShort   K, L;
    FT_F26Dot6  D;


    K = (FT_UShort)args[1];
    L = (FT_UShort)args[0];

    if ( BOUNDS( L, exc->zp0.n_points ) ||
         BOUNDS( K, exc->zp1.n_points ) )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      D = 0;
    }
    else
    {
      if ( exc->opcode & 1 )
        D = PROJECT( exc->zp0.cur + L, exc->zp1.cur + K );
      else
      {
        /* XXX: UNDOCUMENTED: twilight zone special case */

        if ( exc->GS.gep0 == 0 || exc->GS.gep1 == 0 )
        {
          FT_Vector*  vec1 = exc->zp0.org + L;
          FT_Vector*  vec2 = exc->zp1.org + K;


          D = DUALPROJ( vec1, vec2 );
        }
        else
        {
          FT_Vector*  vec1 = exc->zp0.orus + L;
          FT_Vector*  vec2 = exc->zp1.orus + K;


          if ( exc->metrics.x_scale == exc->metrics.y_scale )
          {
            /* this should be faster */
            D = DUALPROJ( vec1, vec2 );
            D = FT_MulFix( D, exc->metrics.x_scale );
          }
          else
          {
            FT_Vector  vec;


            vec.x = FT_MulFix( vec1->x - vec2->x, exc->metrics.x_scale );
            vec.y = FT_MulFix( vec1->y - vec2->y, exc->metrics.y_scale );

            D = FAST_DUALPROJ( &vec );
          }
        }
      }
    }

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    /* Disable Type 2 Vacuform Rounds - e.g. Arial Narrow */
    if ( SUBPIXEL_HINTING_INFINALITY &&
         exc->ignore_x_mode          &&
         FT_ABS( D ) == 64           )
      D += 1;
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

    args[0] = D;
  }

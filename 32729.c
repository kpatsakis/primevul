  Ins_MD( INS_ARG )
  {
    FT_UShort   K, L;
    FT_F26Dot6  D;


    K = (FT_UShort)args[1];
    L = (FT_UShort)args[0];

    if( BOUNDS( L, CUR.zp0.n_points ) ||
        BOUNDS( K, CUR.zp1.n_points ) )
    {
      if ( CUR.pedantic_hinting )
      {
        CUR.error = TT_Err_Invalid_Reference;
        return;
      }
      D = 0;
    }
    else
    {
      if ( CUR.opcode & 1 )
        D = CUR_Func_project( CUR.zp0.cur + L, CUR.zp1.cur + K );
      else
      {
        FT_Vector*  vec1 = CUR.zp0.orus + L;
        FT_Vector*  vec2 = CUR.zp1.orus + K;


        if ( CUR.metrics.x_scale == CUR.metrics.y_scale )
        {
          /* this should be faster */
          D = CUR_Func_dualproj( vec1, vec2 );
          D = TT_MULFIX( D, CUR.metrics.x_scale );
        }
        else
        {
          FT_Vector  vec;


          vec.x = TT_MULFIX( vec1->x - vec2->x, CUR.metrics.x_scale );
          vec.y = TT_MULFIX( vec1->y - vec2->y, CUR.metrics.y_scale );

          D = CUR_fast_dualproj( &vec );
        }
      }
    }

    args[0] = D;
  }

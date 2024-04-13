  Ins_MDRP( INS_ARG )
  {
    FT_UShort   point;
    FT_F26Dot6  org_dist, distance;


    point = (FT_UShort)args[0];

    if ( BOUNDS( point,      CUR.zp1.n_points ) ||
         BOUNDS( CUR.GS.rp0, CUR.zp0.n_points ) )
    {
      if ( CUR.pedantic_hinting )
        CUR.error = TT_Err_Invalid_Reference;
      return;
    }

    /* XXX: Is there some undocumented feature while in the */
    /*      twilight zone?                                  */

    /* XXX: UNDOCUMENTED: twilight zone special case */

    if ( CUR.GS.gep0 == 0 || CUR.GS.gep1 == 0 )
    {
      FT_Vector*  vec1 = &CUR.zp1.org[point];
      FT_Vector*  vec2 = &CUR.zp0.org[CUR.GS.rp0];


      org_dist = CUR_Func_dualproj( vec1, vec2 );
    }
    else
    {
      FT_Vector*  vec1 = &CUR.zp1.orus[point];
      FT_Vector*  vec2 = &CUR.zp0.orus[CUR.GS.rp0];


      if ( CUR.metrics.x_scale == CUR.metrics.y_scale )
      {
        /* this should be faster */
        org_dist = CUR_Func_dualproj( vec1, vec2 );
        org_dist = TT_MULFIX( org_dist, CUR.metrics.x_scale );
      }
      else
      {
        FT_Vector  vec;


        vec.x = TT_MULFIX( vec1->x - vec2->x, CUR.metrics.x_scale );
        vec.y = TT_MULFIX( vec1->y - vec2->y, CUR.metrics.y_scale );

        org_dist = CUR_fast_dualproj( &vec );
      }
    }

    /* single width cut-in test */

    if ( FT_ABS( org_dist - CUR.GS.single_width_value ) <
         CUR.GS.single_width_cutin )
    {
      if ( org_dist >= 0 )
        org_dist = CUR.GS.single_width_value;
      else
        org_dist = -CUR.GS.single_width_value;
    }

    /* round flag */

    if ( ( CUR.opcode & 4 ) != 0 )
      distance = CUR_Func_round(
                   org_dist,
                   CUR.tt_metrics.compensations[CUR.opcode & 3] );
    else
      distance = ROUND_None(
                   org_dist,
                   CUR.tt_metrics.compensations[CUR.opcode & 3] );

    /* minimum distance flag */

    if ( ( CUR.opcode & 8 ) != 0 )
    {
      if ( org_dist >= 0 )
      {
        if ( distance < CUR.GS.minimum_distance )
          distance = CUR.GS.minimum_distance;
      }
      else
      {
        if ( distance > -CUR.GS.minimum_distance )
          distance = -CUR.GS.minimum_distance;
      }
    }

    /* now move the point */

    org_dist = CUR_Func_project( CUR.zp1.cur + point,
                                 CUR.zp0.cur + CUR.GS.rp0 );

    CUR_Func_move( &CUR.zp1, point, distance - org_dist );

    CUR.GS.rp1 = CUR.GS.rp0;
    CUR.GS.rp2 = point;

    if ( ( CUR.opcode & 16 ) != 0 )
      CUR.GS.rp0 = point;
  }

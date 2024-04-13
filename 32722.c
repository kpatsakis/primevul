  Ins_IP( INS_ARG )
  {
    FT_F26Dot6  old_range, cur_range;
    FT_Vector*  orus_base;
    FT_Vector*  cur_base;
    FT_Int      twilight;

    FT_UNUSED_ARG;


    if ( CUR.top < CUR.GS.loop )
    {
      CUR.error = TT_Err_Invalid_Reference;
      return;
    }

    /*
     * We need to deal in a special way with the twilight zone.
     * Otherwise, by definition, the value of CUR.twilight.orus[n] is (0,0),
     * for every n.
     */
    twilight = CUR.GS.gep0 == 0 || CUR.GS.gep1 == 0 || CUR.GS.gep2 == 0;

    if ( BOUNDS( CUR.GS.rp1, CUR.zp0.n_points ) )
    {
      if ( CUR.pedantic_hinting )
        CUR.error = TT_Err_Invalid_Reference;
      return;
    }

    if ( twilight )
      orus_base = &CUR.zp0.org[CUR.GS.rp1];
    else
      orus_base = &CUR.zp0.orus[CUR.GS.rp1];

    cur_base = &CUR.zp0.cur[CUR.GS.rp1];

    /* XXX: There are some glyphs in some braindead but popular */
    /*      fonts out there (e.g. [aeu]grave in monotype.ttf)   */
    /*      calling IP[] with bad values of rp[12].             */
    /*      Do something sane when this odd thing happens.      */
    if ( BOUNDS( CUR.GS.rp1, CUR.zp0.n_points ) ||
         BOUNDS( CUR.GS.rp2, CUR.zp1.n_points ) )
    {
      old_range = 0;
      cur_range = 0;
    }
    else
    {
      if ( twilight )
        old_range = CUR_Func_dualproj( &CUR.zp1.org[CUR.GS.rp2],
                                       orus_base );
      else
        old_range = CUR_Func_dualproj( &CUR.zp1.orus[CUR.GS.rp2],
                                       orus_base );

      cur_range = CUR_Func_project ( &CUR.zp1.cur[CUR.GS.rp2], cur_base );
    }

    for ( ; CUR.GS.loop > 0; --CUR.GS.loop )
    {
      FT_UInt     point = (FT_UInt)CUR.stack[--CUR.args];
      FT_F26Dot6  org_dist, cur_dist, new_dist;


      /* check point bounds */
      if ( BOUNDS( point, CUR.zp2.n_points ) )
      {
        if ( CUR.pedantic_hinting )
        {
          CUR.error = TT_Err_Invalid_Reference;
          return;
        }
        continue;
      }

      if ( twilight )
        org_dist = CUR_Func_dualproj( &CUR.zp2.org[point], orus_base );
      else
        org_dist = CUR_Func_dualproj( &CUR.zp2.orus[point], orus_base );

      cur_dist = CUR_Func_project ( &CUR.zp2.cur[point], cur_base );

      if ( org_dist )
        new_dist = ( old_range != 0 )
                     ? TT_MULDIV( org_dist, cur_range, old_range )
                     : cur_dist;
      else
        new_dist = 0;

      CUR_Func_move( &CUR.zp2, (FT_UShort)point, new_dist - cur_dist );
    }
    CUR.GS.loop = 1;
    CUR.new_top = CUR.args;
  }

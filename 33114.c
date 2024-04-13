  _iup_worker_interpolate( IUP_Worker  worker,
                           FT_UInt     p1,
                           FT_UInt     p2,
                           FT_UInt     ref1,
                           FT_UInt     ref2 )
  {
    FT_UInt     i;
    FT_F26Dot6  orus1, orus2, org1, org2, cur1, cur2, delta1, delta2;


    if ( p1 > p2 )
      return;

    if ( BOUNDS( ref1, worker->max_points ) ||
         BOUNDS( ref2, worker->max_points ) )
      return;

    orus1 = worker->orus[ref1].x;
    orus2 = worker->orus[ref2].x;

    if ( orus1 > orus2 )
    {
      FT_F26Dot6  tmp_o;
      FT_UInt     tmp_r;


      tmp_o = orus1;
      orus1 = orus2;
      orus2 = tmp_o;

      tmp_r = ref1;
      ref1  = ref2;
      ref2  = tmp_r;
    }

    org1   = worker->orgs[ref1].x;
    org2   = worker->orgs[ref2].x;
    cur1   = worker->curs[ref1].x;
    cur2   = worker->curs[ref2].x;
    delta1 = SUB_LONG( cur1, org1 );
    delta2 = SUB_LONG( cur2, org2 );

    if ( cur1 == cur2 || orus1 == orus2 )
    {

      /* trivial snap or shift of untouched points */
      for ( i = p1; i <= p2; i++ )
      {
        FT_F26Dot6  x = worker->orgs[i].x;


        if ( x <= org1 )
          x = ADD_LONG( x, delta1 );

        else if ( x >= org2 )
          x = ADD_LONG( x, delta2 );

        else
          x = cur1;

        worker->curs[i].x = x;
      }
    }
    else
    {
      FT_Fixed  scale       = 0;
      FT_Bool   scale_valid = 0;


      /* interpolation */
      for ( i = p1; i <= p2; i++ )
      {
        FT_F26Dot6  x = worker->orgs[i].x;


        if ( x <= org1 )
          x = ADD_LONG( x, delta1 );

        else if ( x >= org2 )
          x = ADD_LONG( x, delta2 );

        else
        {
          if ( !scale_valid )
          {
            scale_valid = 1;
            scale       = FT_DivFix( SUB_LONG( cur2, cur1 ),
                                     SUB_LONG( orus2, orus1 ) );
          }

          x = ADD_LONG( cur1,
                        FT_MulFix( SUB_LONG( worker->orus[i].x, orus1 ),
                                   scale ) );
        }
        worker->curs[i].x = x;
      }
    }
  }

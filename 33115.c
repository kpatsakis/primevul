  _iup_worker_shift( IUP_Worker  worker,
                     FT_UInt     p1,
                     FT_UInt     p2,
                     FT_UInt     p )
  {
    FT_UInt     i;
    FT_F26Dot6  dx;


    dx = SUB_LONG( worker->curs[p].x, worker->orgs[p].x );
    if ( dx != 0 )
    {
      for ( i = p1; i < p; i++ )
        worker->curs[i].x = ADD_LONG( worker->curs[i].x, dx );

      for ( i = p + 1; i <= p2; i++ )
        worker->curs[i].x = ADD_LONG( worker->curs[i].x, dx );
    }
  }

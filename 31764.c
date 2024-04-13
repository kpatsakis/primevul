  Draw_Sweep( RAS_ARG )
  {
    Short         y, y_change, y_height;

    PProfile      P, Q, P_Left, P_Right;

    Short         min_Y, max_Y, top, bottom, dropouts;

    Long          x1, x2, xs, e1, e2;

    TProfileList  waiting;
    TProfileList  draw_left, draw_right;


    /* initialize empty linked lists */

    Init_Linked( &waiting );

    Init_Linked( &draw_left  );
    Init_Linked( &draw_right );

    /* first, compute min and max Y */

    P     = ras.fProfile;
    max_Y = (Short)TRUNC( ras.minY );
    min_Y = (Short)TRUNC( ras.maxY );

    while ( P )
    {
      Q = P->link;

      bottom = (Short)P->start;
      top    = (Short)( P->start + P->height - 1 );

      if ( min_Y > bottom )
        min_Y = bottom;
      if ( max_Y < top )
        max_Y = top;

      P->X = 0;
      InsNew( &waiting, P );

      P = Q;
    }

    /* check the Y-turns */
    if ( ras.numTurns == 0 )
    {
      ras.error = FT_THROW( Invalid );
      return FAILURE;
    }

    /* now initialize the sweep */

    ras.Proc_Sweep_Init( RAS_VARS &min_Y, &max_Y );

    /* then compute the distance of each profile from min_Y */

    P = waiting;

    while ( P )
    {
      P->countL = (UShort)( P->start - min_Y );
      P = P->link;
    }

    /* let's go */

    y        = min_Y;
    y_height = 0;

    if ( ras.numTurns > 0                     &&
         ras.sizeBuff[-ras.numTurns] == min_Y )
      ras.numTurns--;

    while ( ras.numTurns > 0 )
    {
      /* check waiting list for new activations */

      P = waiting;

      while ( P )
      {
        Q = P->link;
        P->countL -= y_height;
        if ( P->countL == 0 )
        {
          DelOld( &waiting, P );

          if ( P->flags & Flow_Up )
            InsNew( &draw_left,  P );
          else
            InsNew( &draw_right, P );
        }

        P = Q;
      }

      /* sort the drawing lists */

      Sort( &draw_left );
      Sort( &draw_right );

      y_change = (Short)ras.sizeBuff[-ras.numTurns--];
      y_height = (Short)( y_change - y );

      while ( y < y_change )
      {
        /* let's trace */

        dropouts = 0;

        P_Left  = draw_left;
        P_Right = draw_right;

        while ( P_Left )
        {
          x1 = P_Left ->X;
          x2 = P_Right->X;

          if ( x1 > x2 )
          {
            xs = x1;
            x1 = x2;
            x2 = xs;
          }

          e1 = FLOOR( x1 );
          e2 = CEILING( x2 );

          if ( x2 - x1 <= ras.precision &&
               e1 != x1 && e2 != x2     )
          {
            if ( e1 > e2 || e2 == e1 + ras.precision )
            {
              Int  dropOutControl = P_Left->flags & 7;


              if ( dropOutControl != 2 )
              {
                /* a drop-out was detected */

                P_Left ->X = x1;
                P_Right->X = x2;

                /* mark profile for drop-out processing */
                P_Left->countL = 1;
                dropouts++;
              }

              goto Skip_To_Next;
            }
          }

          ras.Proc_Sweep_Span( RAS_VARS y, x1, x2, P_Left, P_Right );

        Skip_To_Next:

          P_Left  = P_Left->link;
          P_Right = P_Right->link;
        }

        /* handle drop-outs _after_ the span drawing --       */
        /* drop-out processing has been moved out of the loop */
        /* for performance tuning                             */
        if ( dropouts > 0 )
          goto Scan_DropOuts;

      Next_Line:

        ras.Proc_Sweep_Step( RAS_VAR );

        y++;

        if ( y < y_change )
        {
          Sort( &draw_left  );
          Sort( &draw_right );
        }
      }

      /* now finalize the profiles that need it */

      P = draw_left;
      while ( P )
      {
        Q = P->link;
        if ( P->height == 0 )
          DelOld( &draw_left, P );
        P = Q;
      }

      P = draw_right;
      while ( P )
      {
        Q = P->link;
        if ( P->height == 0 )
          DelOld( &draw_right, P );
        P = Q;
      }
    }

    /* for gray-scaling, flush the bitmap scanline cache */
    while ( y <= max_Y )
    {
      ras.Proc_Sweep_Step( RAS_VAR );
      y++;
    }

    return SUCCESS;

  Scan_DropOuts:

    P_Left  = draw_left;
    P_Right = draw_right;

    while ( P_Left )
    {
      if ( P_Left->countL )
      {
        P_Left->countL = 0;
#if 0
        dropouts--;  /* -- this is useful when debugging only */
#endif
        ras.Proc_Sweep_Drop( RAS_VARS y,
                                      P_Left->X,
                                      P_Right->X,
                                      P_Left,
                                      P_Right );
      }

      P_Left  = P_Left->link;
      P_Right = P_Right->link;
    }

    goto Next_Line;
  }

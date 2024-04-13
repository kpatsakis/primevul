  gray_render_cubic( RAS_ARG_ const FT_Vector*  control1,
                              const FT_Vector*  control2,
                              const FT_Vector*  to )
  {
    int         top, level;
    int*        levels;
    FT_Vector*  arc;
    int         mid_x = ( DOWNSCALE( ras.x ) + to->x +
                          3 * (control1->x + control2->x ) ) / 8;
    int         mid_y = ( DOWNSCALE( ras.y ) + to->y +
                          3 * (control1->y + control2->y ) ) / 8;
    TPos        dx = DOWNSCALE( ras.x ) + to->x - ( mid_x << 1 );
    TPos        dy = DOWNSCALE( ras.y ) + to->y - ( mid_y << 1 );


    if ( dx < 0 )
      dx = -dx;
    if ( dy < 0 )
      dy = -dy;
    if ( dx < dy )
      dx = dy;

    level = 1;
    dx /= ras.cubic_level;
    while ( dx > 0 )
    {
      dx >>= 2;
      level++;
    }

    if ( level <= 1 )
    {
      TPos  to_x, to_y;


      to_x  = UPSCALE( to->x );
      to_y  = UPSCALE( to->y );

      /* Recalculation of midpoint is needed only if */
      /* UPSCALE and DOWNSCALE have any effect.      */

#if ( PIXEL_BITS != 6 )
      mid_x = ( ras.x + to_x +
                3 * UPSCALE( control1->x + control2->x ) ) / 8;
      mid_y = ( ras.y + to_y +
                3 * UPSCALE( control1->y + control2->y ) ) / 8;
#endif

      gray_render_line( RAS_VAR_ mid_x, mid_y );
      gray_render_line( RAS_VAR_ to_x, to_y );

      return;
    }

    arc      = ras.bez_stack;
    arc[0].x = UPSCALE( to->x );
    arc[0].y = UPSCALE( to->y );
    arc[1].x = UPSCALE( control2->x );
    arc[1].y = UPSCALE( control2->y );
    arc[2].x = UPSCALE( control1->x );
    arc[2].y = UPSCALE( control1->y );
    arc[3].x = ras.x;
    arc[3].y = ras.y;

    levels    = ras.lev_stack;
    top       = 0;
    levels[0] = level;

    while ( top >= 0 )
    {
      level = levels[top];
      if ( level > 1 )
      {
        /* check that the arc crosses the current band */
        TPos  min, max, y;


        min = max = arc[0].y;
        y = arc[1].y;
        if ( y < min ) min = y;
        if ( y > max ) max = y;
        y = arc[2].y;
        if ( y < min ) min = y;
        if ( y > max ) max = y;
        y = arc[3].y;
        if ( y < min ) min = y;
        if ( y > max ) max = y;
        if ( TRUNC( min ) >= ras.max_ey || TRUNC( max ) < 0 )
          goto Draw;
        gray_split_cubic( arc );
        arc += 3;
        top ++;
        levels[top] = levels[top - 1] = level - 1;
        continue;
      }

    Draw:
      {
        TPos  to_x, to_y;


        to_x  = arc[0].x;
        to_y  = arc[0].y;
        mid_x = ( ras.x + to_x + 3 * ( arc[1].x + arc[2].x ) ) / 8;
        mid_y = ( ras.y + to_y + 3 * ( arc[1].y + arc[2].y ) ) / 8;

        gray_render_line( RAS_VAR_ mid_x, mid_y );
        gray_render_line( RAS_VAR_ to_x, to_y );
        top --;
        arc -= 3;
      }
    }

    return;
  }

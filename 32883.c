  gray_hline( RAS_ARG_ TCoord  x,
                       TCoord  y,
                       TPos    area,
                       TCoord  acount )
  {
    FT_Span*  span;
    int       count;
    int       coverage;


    /* compute the coverage line's coverage, depending on the    */
    /* outline fill rule                                         */
    /*                                                           */
    /* the coverage percentage is area/(PIXEL_BITS*PIXEL_BITS*2) */
    /*                                                           */
    coverage = (int)( area >> ( PIXEL_BITS * 2 + 1 - 8 ) );
                                                    /* use range 0..256 */
    if ( coverage < 0 )
      coverage = -coverage;

    if ( ras.outline.flags & FT_OUTLINE_EVEN_ODD_FILL )
    {
      coverage &= 511;

      if ( coverage > 256 )
        coverage = 512 - coverage;
      else if ( coverage == 256 )
        coverage = 255;
    }
    else
    {
      /* normal non-zero winding rule */
      if ( coverage >= 256 )
        coverage = 255;
    }

    y += (TCoord)ras.min_ey;
    x += (TCoord)ras.min_ex;

    /* FT_Span.x is a 16-bit short, so limit our coordinates appropriately */
    if ( x >= 32767 )
      x = 32767;

    /* FT_Span.y is an integer, so limit our coordinates appropriately */
    if ( y >= FT_INT_MAX )
      y = FT_INT_MAX;

    if ( coverage )
    {
      /* see whether we can add this span to the current list */
      count = ras.num_gray_spans;
      span  = ras.gray_spans + count - 1;
      if ( count > 0                          &&
           ras.span_y == y                    &&
           (int)span->x + span->len == (int)x &&
           span->coverage == coverage         )
      {
        span->len = (unsigned short)( span->len + acount );
        return;
      }

      if ( ras.span_y != y || count >= FT_MAX_GRAY_SPANS )
      {
        if ( ras.render_span && count > 0 )
          ras.render_span( ras.span_y, count, ras.gray_spans,
                           ras.render_span_data );

#ifdef FT_DEBUG_LEVEL_TRACE

        if ( count > 0 )
        {
          int  n;


          FT_TRACE7(( "y = %3d ", ras.span_y ));
          span = ras.gray_spans;
          for ( n = 0; n < count; n++, span++ )
            FT_TRACE7(( "[%d..%d]:%02x ",
                        span->x, span->x + span->len - 1, span->coverage ));
          FT_TRACE7(( "\n" ));
        }

#endif /* FT_DEBUG_LEVEL_TRACE */

        ras.num_gray_spans = 0;
        ras.span_y         = (int)y;

        count = 0;
        span  = ras.gray_spans;
      }
      else
        span++;

      /* add a gray span to the current list */
      span->x        = (short)x;
      span->len      = (unsigned short)acount;
      span->coverage = (unsigned char)coverage;

      ras.num_gray_spans++;
    }
  }

  gray_sweep( RAS_ARG_ const FT_Bitmap*  target )
  {
    int  yindex;

    FT_UNUSED( target );


    if ( ras.num_cells == 0 )
      return;

    ras.num_gray_spans = 0;

    FT_TRACE7(( "gray_sweep: start\n" ));

    for ( yindex = 0; yindex < ras.ycount; yindex++ )
    {
      PCell   cell  = ras.ycells[yindex];
      TCoord  cover = 0;
      TCoord  x     = 0;


      for ( ; cell != NULL; cell = cell->next )
      {
        TPos  area;


        if ( cell->x > x && cover != 0 )
          gray_hline( RAS_VAR_ x, yindex, cover * ( ONE_PIXEL * 2 ),
                      cell->x - x );

        cover += cell->cover;
        area   = cover * ( ONE_PIXEL * 2 ) - cell->area;

        if ( area != 0 && cell->x >= 0 )
          gray_hline( RAS_VAR_ cell->x, yindex, area, 1 );

        x = cell->x + 1;
      }

      if ( cover != 0 )
        gray_hline( RAS_VAR_ x, yindex, cover * ( ONE_PIXEL * 2 ),
                    ras.count_ex - x );
    }

    if ( ras.render_span && ras.num_gray_spans > 0 )
      ras.render_span( ras.span_y, ras.num_gray_spans,
                       ras.gray_spans, ras.render_span_data );

    FT_TRACE7(( "gray_sweep: end\n" ));
  }

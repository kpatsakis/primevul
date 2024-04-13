  Vertical_Sweep_Init( RAS_ARGS Short*  min,
                                Short*  max )
  {
    Long  pitch = ras.target.pitch;

    FT_UNUSED( max );


    ras.traceIncr = (Short)-pitch;
    ras.traceOfs  = -*min * pitch;
    if ( pitch > 0 )
      ras.traceOfs += ( ras.target.rows - 1 ) * pitch;

    ras.gray_min_x = 0;
    ras.gray_max_x = 0;
  }

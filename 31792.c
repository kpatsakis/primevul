  ft_black_init( black_PRaster  raster )
  {
#ifdef FT_RASTER_OPTION_ANTI_ALIASING
    FT_UInt  n;


    /* set default 5-levels gray palette */
    for ( n = 0; n < 5; n++ )
      raster->grays[n] = n * 255 / 4;

    raster->gray_width = RASTER_GRAY_LINES / 2;
#else
    FT_UNUSED( raster );
#endif
  }

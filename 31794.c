  ft_black_new( FT_Memory       memory,
                black_PRaster  *araster )
  {
    FT_Error       error;
    black_PRaster  raster = NULL;


    *araster = 0;
    if ( !FT_NEW( raster ) )
    {
      raster->memory = memory;
      ft_black_init( raster );

      *araster = raster;
    }

    return error;
  }

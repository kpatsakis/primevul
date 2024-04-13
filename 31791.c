  ft_black_done( black_PRaster  raster )
  {
    FT_Memory  memory = (FT_Memory)raster->memory;


    FT_FREE( raster );
  }

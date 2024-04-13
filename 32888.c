  gray_raster_new( FT_Memory   memory,
                   FT_Raster*  araster )
  {
    FT_Error  error;
    PRaster   raster;


    *araster = 0;
    if ( !FT_ALLOC( raster, sizeof ( TRaster ) ) )
    {
      raster->memory = memory;
      *araster = (FT_Raster)raster;
    }

    return error;
  }

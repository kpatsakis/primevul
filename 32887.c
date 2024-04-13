  gray_raster_new( void*       memory,
                   FT_Raster*  araster )
  {
    static TRaster  the_raster;

    FT_UNUSED( memory );


    *araster = (FT_Raster)&the_raster;
    FT_MEM_ZERO( &the_raster, sizeof ( the_raster ) );

    return 0;
  }

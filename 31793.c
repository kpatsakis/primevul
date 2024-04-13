  ft_black_new( void*       memory,
                FT_Raster  *araster )
  {
     static black_TRaster  the_raster;
     FT_UNUSED( memory );


     *araster = (FT_Raster)&the_raster;
     FT_MEM_ZERO( &the_raster, sizeof ( the_raster ) );
     ft_black_init( &the_raster );

     return 0;
  }

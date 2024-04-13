  ft_black_set_mode( black_PRaster  raster,
                     unsigned long  mode,
                     const char*    palette )
  {
#ifdef FT_RASTER_OPTION_ANTI_ALIASING

    if ( mode == FT_MAKE_TAG( 'p', 'a', 'l', '5' ) )
    {
      /* set 5-levels gray palette */
      raster->grays[0] = palette[0];
      raster->grays[1] = palette[1];
      raster->grays[2] = palette[2];
      raster->grays[3] = palette[3];
      raster->grays[4] = palette[4];
    }

#else

    FT_UNUSED( raster );
    FT_UNUSED( mode );
    FT_UNUSED( palette );

#endif
  }

  Render_Single_Pass( RAS_ARGS Bool  flipped )
  {
    Short  i, j, k;


    while ( ras.band_top >= 0 )
    {
      ras.maxY = (Long)ras.band_stack[ras.band_top].y_max * ras.precision;
      ras.minY = (Long)ras.band_stack[ras.band_top].y_min * ras.precision;

      ras.top = ras.buff;

      ras.error = Raster_Err_None;

      if ( Convert_Glyph( RAS_VARS flipped ) )
      {
        if ( ras.error != Raster_Err_Overflow )
          return FAILURE;

        ras.error = Raster_Err_None;

        /* sub-banding */

#ifdef DEBUG_RASTER
        ClearBand( RAS_VARS TRUNC( ras.minY ), TRUNC( ras.maxY ) );
#endif

        i = ras.band_stack[ras.band_top].y_min;
        j = ras.band_stack[ras.band_top].y_max;

        k = (Short)( ( i + j ) / 2 );

        if ( ras.band_top >= 7 || k < i )
        {
          ras.band_top = 0;
          ras.error    = FT_THROW( Invalid );

          return ras.error;
        }

        ras.band_stack[ras.band_top + 1].y_min = k;
        ras.band_stack[ras.band_top + 1].y_max = j;

        ras.band_stack[ras.band_top].y_max = (Short)( k - 1 );

        ras.band_top++;
      }
      else
      {
        if ( ras.fProfile )
          if ( Draw_Sweep( RAS_VAR ) )
             return ras.error;
        ras.band_top--;
      }
    }

    return SUCCESS;
  }

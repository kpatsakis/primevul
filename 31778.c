  Render_Gray_Glyph( RAS_ARG )
  {
    Long      pixel_width;
    FT_Error  error;


    Set_High_Precision( RAS_VARS ras.outline.flags &
                                 FT_OUTLINE_HIGH_PRECISION );
    ras.scale_shift = ras.precision_shift + 1;

    if ( ras.outline.flags & FT_OUTLINE_IGNORE_DROPOUTS )
      ras.dropOutControl = 2;
    else
    {
      if ( ras.outline.flags & FT_OUTLINE_SMART_DROPOUTS )
        ras.dropOutControl = 4;
      else
        ras.dropOutControl = 0;

      if ( !( ras.outline.flags & FT_OUTLINE_INCLUDE_STUBS ) )
        ras.dropOutControl += 1;
    }

    ras.second_pass = !( ras.outline.flags & FT_OUTLINE_SINGLE_PASS );

    /* Vertical Sweep */

    ras.band_top            = 0;
    ras.band_stack[0].y_min = 0;
    ras.band_stack[0].y_max = 2 * ras.target.rows - 1;

    ras.bWidth  = ras.gray_width;
    pixel_width = 2 * ( ( ras.target.width + 3 ) >> 2 );

    if ( ras.bWidth > pixel_width )
      ras.bWidth = pixel_width;

    ras.bWidth  = ras.bWidth * 8;
    ras.bTarget = (Byte*)ras.gray_lines;
    ras.gTarget = (Byte*)ras.target.buffer;

    ras.Proc_Sweep_Init = Vertical_Gray_Sweep_Init;
    ras.Proc_Sweep_Span = Vertical_Sweep_Span;
    ras.Proc_Sweep_Drop = Vertical_Sweep_Drop;
    ras.Proc_Sweep_Step = Vertical_Gray_Sweep_Step;

    error = Render_Single_Pass( RAS_VARS 0 );
    if ( error )
      return error;

    /* Horizontal Sweep */
    if ( ras.second_pass && ras.dropOutControl != 2 )
    {
      ras.Proc_Sweep_Init = Horizontal_Sweep_Init;
      ras.Proc_Sweep_Span = Horizontal_Gray_Sweep_Span;
      ras.Proc_Sweep_Drop = Horizontal_Gray_Sweep_Drop;
      ras.Proc_Sweep_Step = Horizontal_Sweep_Step;

      ras.band_top            = 0;
      ras.band_stack[0].y_min = 0;
      ras.band_stack[0].y_max = ras.target.width * 2 - 1;

      error = Render_Single_Pass( RAS_VARS 1 );
      if ( error )
        return error;
    }

    return Raster_Err_None;
  }

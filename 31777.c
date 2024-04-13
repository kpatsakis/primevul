  Render_Glyph( RAS_ARG )
  {
    FT_Error  error;


    Set_High_Precision( RAS_VARS ras.outline.flags &
                                 FT_OUTLINE_HIGH_PRECISION );
    ras.scale_shift = ras.precision_shift;

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

    ras.second_pass = (FT_Byte)( !( ras.outline.flags &
                                    FT_OUTLINE_SINGLE_PASS ) );

    /* Vertical Sweep */
    ras.Proc_Sweep_Init = Vertical_Sweep_Init;
    ras.Proc_Sweep_Span = Vertical_Sweep_Span;
    ras.Proc_Sweep_Drop = Vertical_Sweep_Drop;
    ras.Proc_Sweep_Step = Vertical_Sweep_Step;

    ras.band_top            = 0;
    ras.band_stack[0].y_min = 0;
    ras.band_stack[0].y_max = (short)( ras.target.rows - 1 );

    ras.bWidth  = (unsigned short)ras.target.width;
    ras.bTarget = (Byte*)ras.target.buffer;

    if ( ( error = Render_Single_Pass( RAS_VARS 0 ) ) != 0 )
      return error;

    /* Horizontal Sweep */
    if ( ras.second_pass && ras.dropOutControl != 2 )
    {
      ras.Proc_Sweep_Init = Horizontal_Sweep_Init;
      ras.Proc_Sweep_Span = Horizontal_Sweep_Span;
      ras.Proc_Sweep_Drop = Horizontal_Sweep_Drop;
      ras.Proc_Sweep_Step = Horizontal_Sweep_Step;

      ras.band_top            = 0;
      ras.band_stack[0].y_min = 0;
      ras.band_stack[0].y_max = (short)( ras.target.width - 1 );

      if ( ( error = Render_Single_Pass( RAS_VARS 1 ) ) != 0 )
        return error;
    }

    return Raster_Err_None;
  }

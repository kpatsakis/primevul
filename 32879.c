  gray_convert_glyph_inner( RAS_ARG )
  {

    volatile int  error = 0;

#ifdef FT_CONFIG_OPTION_PIC
      FT_Outline_Funcs func_interface;
      Init_Class_func_interface(&func_interface);
#endif

    if ( ft_setjmp( ras.jump_buffer ) == 0 )
    {
      error = FT_Outline_Decompose( &ras.outline, &func_interface, &ras );
      gray_record_cell( RAS_VAR );
    }
    else
      error = ErrRaster_Memory_Overflow;

    return error;
  }

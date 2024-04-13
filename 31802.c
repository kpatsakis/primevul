  warning_callback( png_structp      png,
                    png_const_charp  error_msg )
  {
    FT_UNUSED( png );
    FT_UNUSED( error_msg );

    /* Just ignore warnings. */
  }

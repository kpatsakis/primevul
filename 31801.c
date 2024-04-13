  read_data_from_FT_Stream( png_structp  png,
                            png_bytep    data,
                            png_size_t   length )
  {
    FT_Error   error;
    png_voidp  p      = png_get_io_ptr( png );
    FT_Stream  stream = (FT_Stream)p;


    if ( FT_FRAME_ENTER( length ) )
    {
      FT_Error*  e = (FT_Error*)png_get_error_ptr( png );


      *e = FT_THROW( Invalid_Stream_Read );
      png_error( png, NULL );

      return;
    }

    memcpy( data, stream->cursor, length );

    FT_FRAME_EXIT();
  }

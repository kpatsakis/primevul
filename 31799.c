  error_callback( png_structp      png,
                  png_const_charp  error_msg )
  {
    FT_Error*  error = (FT_Error*)png_get_error_ptr( png );

    FT_UNUSED( error_msg );


    *error = FT_THROW( Out_Of_Memory );
#ifdef PNG_SETJMP_SUPPORTED
    ft_longjmp( png_jmpbuf( png ), 1 );
#endif
    /* if we get here, then we have no choice but to abort ... */
  }

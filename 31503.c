  _bdf_set_default_spacing( bdf_font_t*     font,
                            bdf_options_t*  opts,
                            unsigned long   lineno )
  {
    size_t       len;
    char         name[256];
    _bdf_list_t  list;
    FT_Memory    memory;
    FT_Error     error = BDF_Err_Ok;


    if ( font == 0 || font->name == 0 || font->name[0] == 0 )
    {
      error = BDF_Err_Invalid_Argument;
      goto Exit;
    }

    memory = font->memory;

    _bdf_list_init( &list, memory );

    font->spacing = opts->font_spacing;

    len = ft_strlen( font->name ) + 1;
    /* Limit ourselves to 256 characters in the font name. */
    if ( len >= 256 )
    {
      FT_ERROR(( "_bdf_set_default_spacing: " ERRMSG7, lineno ));
      error = BDF_Err_Invalid_Argument;
      goto Exit;
    }

    FT_MEM_COPY( name, font->name, len );

    error = _bdf_list_split( &list, (char *)"-", name, len );
    if ( error )
      goto Fail;

    if ( list.used == 15 )
    {
      switch ( list.field[11][0] )
      {
      case 'C':
      case 'c':
        font->spacing = BDF_CHARCELL;
        break;
      case 'M':
      case 'm':
        font->spacing = BDF_MONOWIDTH;
        break;
      case 'P':
      case 'p':
        font->spacing = BDF_PROPORTIONAL;
        break;
      }
    }

  Fail:
    _bdf_list_done( &list );

  Exit:
    return error;
  }

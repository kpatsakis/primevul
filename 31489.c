  _bdf_add_property( bdf_font_t*    font,
                     char*          name,
                     char*          value,
                     unsigned long  lineno )
  {
    size_t          propid;
    hashnode        hn;
    bdf_property_t  *prop, *fp;
    FT_Memory       memory = font->memory;
    FT_Error        error = BDF_Err_Ok;


    /* First, check whether the property already exists in the font. */
    if ( ( hn = hash_lookup( name, (hashtable *)font->internal ) ) != 0 )
    {
      /* The property already exists in the font, so simply replace */
      /* the value of the property with the current value.          */
      fp = font->props + hn->data;

      switch ( fp->format )
      {
      case BDF_ATOM:
        /* Delete the current atom if it exists. */
        FT_FREE( fp->value.atom );

        if ( value && value[0] != 0 )
        {
          if ( FT_STRDUP( fp->value.atom, value ) )
            goto Exit;
        }
        break;

      case BDF_INTEGER:
        fp->value.l = _bdf_atol( value, 0, 10 );
        break;

      case BDF_CARDINAL:
        fp->value.ul = _bdf_atoul( value, 0, 10 );
        break;

      default:
        ;
      }

      goto Exit;
    }

    /* See whether this property type exists yet or not. */
    /* If not, create it.                                */
    hn = hash_lookup( name, &(font->proptbl) );
    if ( hn == 0 )
    {
      error = bdf_create_property( name, BDF_ATOM, font );
      if ( error )
        goto Exit;
      hn = hash_lookup( name, &(font->proptbl) );
    }

    /* Allocate another property if this is overflow. */
    if ( font->props_used == font->props_size )
    {
      if ( font->props_size == 0 )
      {
        if ( FT_NEW_ARRAY( font->props, 1 ) )
          goto Exit;
      }
      else
      {
        if ( FT_RENEW_ARRAY( font->props,
                             font->props_size,
                             font->props_size + 1 ) )
          goto Exit;
      }

      fp = font->props + font->props_size;
      FT_MEM_ZERO( fp, sizeof ( bdf_property_t ) );
      font->props_size++;
    }

    propid = hn->data;
    if ( propid >= _num_bdf_properties )
      prop = font->user_props + ( propid - _num_bdf_properties );
    else
      prop = (bdf_property_t*)_bdf_properties + propid;

    fp = font->props + font->props_used;

    fp->name    = prop->name;
    fp->format  = prop->format;
    fp->builtin = prop->builtin;

    switch ( prop->format )
    {
    case BDF_ATOM:
      fp->value.atom = 0;
      if ( value != 0 && value[0] )
      {
        if ( FT_STRDUP( fp->value.atom, value ) )
          goto Exit;
      }
      break;

    case BDF_INTEGER:
      fp->value.l = _bdf_atol( value, 0, 10 );
      break;

    case BDF_CARDINAL:
      fp->value.ul = _bdf_atoul( value, 0, 10 );
      break;
    }

    /* If the property happens to be a comment, then it doesn't need */
    /* to be added to the internal hash table.                       */
    if ( ft_memcmp( name, "COMMENT", 7 ) != 0 )
    {
      /* Add the property to the font property table. */
      error = hash_insert( fp->name,
                           font->props_used,
                           (hashtable *)font->internal,
                           memory );
      if ( error )
        goto Exit;
    }

    font->props_used++;

    /* Some special cases need to be handled here.  The DEFAULT_CHAR       */
    /* property needs to be located if it exists in the property list, the */
    /* FONT_ASCENT and FONT_DESCENT need to be assigned if they are        */
    /* present, and the SPACING property should override the default       */
    /* spacing.                                                            */
    if ( ft_memcmp( name, "DEFAULT_CHAR", 12 ) == 0 )
      font->default_char = fp->value.l;
    else if ( ft_memcmp( name, "FONT_ASCENT", 11 ) == 0 )
      font->font_ascent = fp->value.l;
    else if ( ft_memcmp( name, "FONT_DESCENT", 12 ) == 0 )
      font->font_descent = fp->value.l;
    else if ( ft_memcmp( name, "SPACING", 7 ) == 0 )
    {
      if ( !fp->value.atom )
      {
        FT_ERROR(( "_bdf_add_property: " ERRMSG8, lineno, "SPACING" ));
        error = BDF_Err_Invalid_File_Format;
        goto Exit;
      }

      if ( fp->value.atom[0] == 'p' || fp->value.atom[0] == 'P' )
        font->spacing = BDF_PROPORTIONAL;
      else if ( fp->value.atom[0] == 'm' || fp->value.atom[0] == 'M' )
        font->spacing = BDF_MONOWIDTH;
      else if ( fp->value.atom[0] == 'c' || fp->value.atom[0] == 'C' )
        font->spacing = BDF_CHARCELL;
    }

  Exit:
    return error;
  }

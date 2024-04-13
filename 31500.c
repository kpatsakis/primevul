  _bdf_parse_properties( char*          line,
                         unsigned long  linelen,
                         unsigned long  lineno,
                         void*          call_data,
                         void*          client_data )
  {
    unsigned long      vlen;
    _bdf_line_func_t*  next;
    _bdf_parse_t*      p;
    char*              name;
    char*              value;
    char               nbuf[128];
    FT_Error           error = BDF_Err_Ok;

    FT_UNUSED( lineno );


    next = (_bdf_line_func_t *)call_data;
    p    = (_bdf_parse_t *)    client_data;

    /* Check for the end of the properties. */
    if ( ft_memcmp( line, "ENDPROPERTIES", 13 ) == 0 )
    {
      /* If the FONT_ASCENT or FONT_DESCENT properties have not been      */
      /* encountered yet, then make sure they are added as properties and */
      /* make sure they are set from the font bounding box info.          */
      /*                                                                  */
      /* This is *always* done regardless of the options, because X11     */
      /* requires these two fields to compile fonts.                      */
      if ( bdf_get_font_property( p->font, "FONT_ASCENT" ) == 0 )
      {
        p->font->font_ascent = p->font->bbx.ascent;
        ft_sprintf( nbuf, "%hd", p->font->bbx.ascent );
        error = _bdf_add_property( p->font, (char *)"FONT_ASCENT",
                                   nbuf, lineno );
        if ( error )
          goto Exit;

        FT_TRACE2(( "_bdf_parse_properties: " ACMSG1, p->font->bbx.ascent ));
        p->font->modified = 1;
      }

      if ( bdf_get_font_property( p->font, "FONT_DESCENT" ) == 0 )
      {
        p->font->font_descent = p->font->bbx.descent;
        ft_sprintf( nbuf, "%hd", p->font->bbx.descent );
        error = _bdf_add_property( p->font, (char *)"FONT_DESCENT",
                                   nbuf, lineno );
        if ( error )
          goto Exit;

        FT_TRACE2(( "_bdf_parse_properties: " ACMSG2, p->font->bbx.descent ));
        p->font->modified = 1;
      }

      p->flags &= ~_BDF_PROPS;
      *next     = _bdf_parse_glyphs;

      goto Exit;
    }

    /* Ignore the _XFREE86_GLYPH_RANGES properties. */
    if ( ft_memcmp( line, "_XFREE86_GLYPH_RANGES", 21 ) == 0 )
      goto Exit;

    /* Handle COMMENT fields and properties in a special way to preserve */
    /* the spacing.                                                      */
    if ( ft_memcmp( line, "COMMENT", 7 ) == 0 )
    {
      name = value = line;
      value += 7;
      if ( *value )
        *value++ = 0;
      error = _bdf_add_property( p->font, name, value, lineno );
      if ( error )
        goto Exit;
    }
    else if ( _bdf_is_atom( line, linelen, &name, &value, p->font ) )
    {
      error = _bdf_add_property( p->font, name, value, lineno );
      if ( error )
        goto Exit;
    }
    else
    {
      error = _bdf_list_split( &p->list, (char *)" +", line, linelen );
      if ( error )
        goto Exit;
      name = p->list.field[0];

      _bdf_list_shift( &p->list, 1 );
      value = _bdf_list_join( &p->list, ' ', &vlen );

      error = _bdf_add_property( p->font, name, value, lineno );
      if ( error )
        goto Exit;
    }

  Exit:
    return error;
  }

  pcf_get_properties( FT_Stream  stream,
                      PCF_Face   face )
  {
    PCF_ParseProperty  props      = 0;
    PCF_Property       properties = NULL;
    FT_ULong           nprops, i;
    FT_ULong           format, size;
    FT_Error           error;
    FT_Memory          memory     = FT_FACE( face )->memory;
    FT_ULong           string_size;
    FT_String*         strings    = 0;


    error = pcf_seek_to_table_type( stream,
                                    face->toc.tables,
                                    face->toc.count,
                                    PCF_PROPERTIES,
                                    &format,
                                    &size );
    if ( error )
      goto Bail;

    if ( FT_READ_ULONG_LE( format ) )
      goto Bail;

    FT_TRACE4(( "pcf_get_properties:\n" ));

    FT_TRACE4(( "  format = %ld\n", format ));

    if ( !PCF_FORMAT_MATCH( format, PCF_DEFAULT_FORMAT ) )
      goto Bail;

    if ( PCF_BYTE_ORDER( format ) == MSBFirst )
      (void)FT_READ_ULONG( nprops );
    else
      (void)FT_READ_ULONG_LE( nprops );
    if ( error )
      goto Bail;

    FT_TRACE4(( "  nprop = %d (truncate %d props)\n",
                (int)nprops, nprops - (int)nprops ));

    nprops = (int)nprops;

    /* rough estimate */
    if ( nprops > size / PCF_PROPERTY_SIZE )
    {
      error = FT_THROW( Invalid_Table );
      goto Bail;
    }

    face->nprops = (int)nprops;

    if ( FT_NEW_ARRAY( props, nprops ) )
      goto Bail;

    for ( i = 0; i < nprops; i++ )
    {
      if ( PCF_BYTE_ORDER( format ) == MSBFirst )
      {
        if ( FT_STREAM_READ_FIELDS( pcf_property_msb_header, props + i ) )
          goto Bail;
      }
      else
      {
        if ( FT_STREAM_READ_FIELDS( pcf_property_header, props + i ) )
          goto Bail;
      }
    }

    /* pad the property array                                            */
    /*                                                                   */
    /* clever here - nprops is the same as the number of odd-units read, */
    /* as only isStringProp are odd length   (Keith Packard)             */
    /*                                                                   */
    if ( nprops & 3 )
    {
      i = 4 - ( nprops & 3 );
      if ( FT_STREAM_SKIP( i ) )
      {
        error = FT_THROW( Invalid_Stream_Skip );
        goto Bail;
      }
    }

    if ( PCF_BYTE_ORDER( format ) == MSBFirst )
      (void)FT_READ_ULONG( string_size );
    else
      (void)FT_READ_ULONG_LE( string_size );
    if ( error )
      goto Bail;

    FT_TRACE4(( "  string_size = %ld\n", string_size ));

    /* rough estimate */
    if ( string_size > size - nprops * PCF_PROPERTY_SIZE )
    {
      error = FT_THROW( Invalid_Table );
      goto Bail;
    }

    /* allocate one more byte so that we have a final null byte */
    if ( FT_NEW_ARRAY( strings, string_size + 1 ) )
      goto Bail;

    error = FT_Stream_Read( stream, (FT_Byte*)strings, string_size );
    if ( error )
      goto Bail;

    if ( FT_NEW_ARRAY( properties, nprops ) )
      goto Bail;

    face->properties = properties;

    for ( i = 0; i < nprops; i++ )
    {
      FT_Long  name_offset = props[i].name;


      if ( ( name_offset < 0 )                     ||
           ( (FT_ULong)name_offset > string_size ) )
      {
        error = FT_THROW( Invalid_Offset );
        goto Bail;
      }

      if ( FT_STRDUP( properties[i].name, strings + name_offset ) )
        goto Bail;

      FT_TRACE4(( "  %s:", properties[i].name ));

      properties[i].isString = props[i].isString;

      if ( props[i].isString )
      {
        FT_Long  value_offset = props[i].value;


        if ( ( value_offset < 0 )                     ||
             ( (FT_ULong)value_offset > string_size ) )
        {
          error = FT_THROW( Invalid_Offset );
          goto Bail;
        }

        if ( FT_STRDUP( properties[i].value.atom, strings + value_offset ) )
          goto Bail;

        FT_TRACE4(( " `%s'\n", properties[i].value.atom ));
      }
      else
      {
        properties[i].value.l = props[i].value;

        FT_TRACE4(( " %d\n", properties[i].value.l ));
      }
    }

    error = FT_Err_Ok;

  Bail:
    FT_FREE( props );
    FT_FREE( strings );

    return error;
  }

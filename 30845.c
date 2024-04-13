  cff_index_get_sid_string( CFF_Index           idx,
                            FT_UInt             sid,
                            FT_Service_PsCMaps  psnames )
  {
    /* value 0xFFFFU indicates a missing dictionary entry */
    if ( sid == 0xFFFFU )
      return 0;

    /* if it is not a standard string, return it */
    if ( sid > 390 )
      return cff_index_get_name( idx, sid - 391 );

    /* CID-keyed CFF fonts don't have glyph names */
    if ( !psnames )
      return 0;

    /* that's a standard string, fetch a copy from the PSName module */
    {
      FT_String*   name       = 0;
      const char*  adobe_name = psnames->adobe_std_strings( sid );


      if ( adobe_name )
      {
        FT_Memory  memory = idx->stream->memory;
        FT_Error   error;


        (void)FT_STRDUP( name, adobe_name );

        FT_UNUSED( error );
      }

      return name;
    }
  }

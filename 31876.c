  tt_check_single_notdef( FT_Face  ttface )
  {
    FT_Bool   result = FALSE;

    TT_Face   face = (TT_Face)ttface;
    FT_UInt   asize;
    FT_ULong  i;
    FT_ULong  glyph_index = 0;
    FT_UInt   count       = 0;


    for( i = 0; i < face->num_locations; i++ )
    {
      tt_face_get_location( face, i, &asize );
      if ( asize > 0 )
      {
        count += 1;
        if ( count > 1 )
          break;
        glyph_index = i;
      }
    }

    /* Only have a single outline. */
    if ( count == 1 )
    {
      if ( glyph_index == 0 )
        result = TRUE;
      else
      {
        /* FIXME: Need to test glyphname == .notdef ? */
        FT_Error error;
        char buf[8];


        error = FT_Get_Glyph_Name( ttface, glyph_index, buf, 8 );
        if ( !error                                            &&
             buf[0] == '.' && !ft_strncmp( buf, ".notdef", 8 ) )
          result = TRUE;
      }
    }

    return result;
  }

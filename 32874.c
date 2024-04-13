  open_face_PS_from_sfnt_stream( FT_Library     library,
                                 FT_Stream      stream,
                                 FT_Long        face_index,
                                 FT_Int         num_params,
                                 FT_Parameter  *params,
                                 FT_Face       *aface )
  {
    FT_Error   error;
    FT_Memory  memory = library->memory;
    FT_ULong   offset, length;
    FT_Long    pos;
    FT_Bool    is_sfnt_cid;
    FT_Byte*   sfnt_ps;

    FT_UNUSED( num_params );
    FT_UNUSED( params );


    pos = FT_Stream_Pos( stream );

    error = ft_lookup_PS_in_sfnt_stream( stream,
                                         face_index,
                                         &offset,
                                         &length,
                                         &is_sfnt_cid );
    if ( error )
      goto Exit;

    if ( FT_Stream_Seek( stream, pos + offset ) )
      goto Exit;

    if ( FT_ALLOC( sfnt_ps, (FT_Long)length ) )
      goto Exit;

    error = FT_Stream_Read( stream, (FT_Byte *)sfnt_ps, length );
    if ( error )
      goto Exit;

    error = open_face_from_buffer( library,
                                   sfnt_ps,
                                   length,
                                   face_index < 0 ? face_index : 0,
                                   is_sfnt_cid ? "cid" : "type1",
                                   aface );
  Exit:
    {
      FT_Error  error1;


      if ( error == FT_Err_Unknown_File_Format )
      {
        error1 = FT_Stream_Seek( stream, pos );
        if ( error1 )
          return error1;
      }

      return error;
    }
  }

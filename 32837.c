  FT_Get_Track_Kerning( FT_Face    face,
                        FT_Fixed   point_size,
                        FT_Int     degree,
                        FT_Fixed*  akerning )
  {
    FT_Service_Kerning  service;
    FT_Error            error = FT_Err_Ok;


    if ( !face )
      return FT_Err_Invalid_Face_Handle;

    if ( !akerning )
      return FT_Err_Invalid_Argument;

    FT_FACE_FIND_SERVICE( face, service, KERNING );
    if ( !service )
      return FT_Err_Unimplemented_Feature;

    error = service->get_track( face,
                                point_size,
                                degree,
                                akerning );

    return error;
  }

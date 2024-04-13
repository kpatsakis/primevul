  FT_Load_Sfnt_Table( FT_Face    face,
                      FT_ULong   tag,
                      FT_Long    offset,
                      FT_Byte*   buffer,
                      FT_ULong*  length )
  {
    FT_Service_SFNT_Table  service;


    if ( !face || !FT_IS_SFNT( face ) )
      return FT_Err_Invalid_Face_Handle;

    FT_FACE_FIND_SERVICE( face, service, SFNT_TABLE );
    if ( service == NULL )
      return FT_Err_Unimplemented_Feature;

    return service->load_table( face, tag, offset, buffer, length );
  }

  FT_Get_Sfnt_Table( FT_Face      face,
                     FT_Sfnt_Tag  tag )
  {
    void*                  table = 0;
    FT_Service_SFNT_Table  service;


    if ( face && FT_IS_SFNT( face ) )
    {
      FT_FACE_FIND_SERVICE( face, service, SFNT_TABLE );
      if ( service != NULL )
        table = service->get_table( face, tag );
    }

    return table;
  }

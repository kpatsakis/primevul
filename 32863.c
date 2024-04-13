  ft_cmap_done_internal( FT_CMap  cmap )
  {
    FT_CMap_Class  clazz  = cmap->clazz;
    FT_Face        face   = cmap->charmap.face;
    FT_Memory      memory = FT_FACE_MEMORY(face);


    if ( clazz->done )
      clazz->done( cmap );

    FT_FREE( cmap );
  }

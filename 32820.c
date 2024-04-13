  FT_Done_Library( FT_Library  library )
  {
    FT_Memory  memory;


    if ( !library )
      return FT_Err_Invalid_Library_Handle;

    memory = library->memory;

    /* Discard client-data */
    if ( library->generic.finalizer )
      library->generic.finalizer( library );

    /* Close all faces in the library.  If we don't do
     * this, we can have some subtle memory leaks.
     * Example:
     *
     *  - the cff font driver uses the pshinter module in cff_size_done
     *  - if the pshinter module is destroyed before the cff font driver,
     *    opened FT_Face objects managed by the driver are not properly
     *    destroyed, resulting in a memory leak
     */
    {
      FT_UInt  n;


      for ( n = 0; n < library->num_modules; n++ )
      {
        FT_Module  module = library->modules[n];
        FT_List    faces;


        if ( ( module->clazz->module_flags & FT_MODULE_FONT_DRIVER ) == 0 )
          continue;

        faces = &FT_DRIVER(module)->faces_list;
        while ( faces->head )
        {
          FT_Done_Face( FT_FACE( faces->head->data ) );
          if ( faces->head )
            FT_TRACE0(( "FT_Done_Library: failed to free some faces\n" ));
        }
      }
    }

    /* Close all other modules in the library */
#if 1
    /* XXX Modules are removed in the reversed order so that  */
    /* type42 module is removed before truetype module.  This */
    /* avoids double free in some occasions.  It is a hack.   */
    while ( library->num_modules > 0 )
      FT_Remove_Module( library,
                        library->modules[library->num_modules - 1] );
#else
    {
      FT_UInt  n;


      for ( n = 0; n < library->num_modules; n++ )
      {
        FT_Module  module = library->modules[n];


        if ( module )
        {
          Destroy_Module( module );
          library->modules[n] = 0;
        }
      }
    }
#endif

    /* Destroy raster objects */
    FT_FREE( library->raster_pool );
    library->raster_pool_size = 0;

#ifdef FT_CONFIG_OPTION_PIC
    /* Destroy pic container contents */
    ft_pic_container_destroy( library );
#endif

    FT_FREE( library );
    return FT_Err_Ok;
  }

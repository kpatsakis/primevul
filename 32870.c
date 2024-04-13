  ft_stub_set_char_sizes( FT_Size     size,
                          FT_F26Dot6  width,
                          FT_F26Dot6  height,
                          FT_UInt     horz_res,
                          FT_UInt     vert_res )
  {
    FT_Size_RequestRec  req;
    FT_Driver           driver = size->face->driver;


    if ( driver->clazz->request_size )
    {
      req.type   = FT_SIZE_REQUEST_TYPE_NOMINAL;
      req.width  = width;
      req.height = height;

      if ( horz_res == 0 )
        horz_res = vert_res;

      if ( vert_res == 0 )
        vert_res = horz_res;

      if ( horz_res == 0 )
        horz_res = vert_res = 72;

      req.horiResolution = horz_res;
      req.vertResolution = vert_res;

      return driver->clazz->request_size( size, &req );
    }

    return 0;
  }

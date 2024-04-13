  ft_black_render( black_PRaster            raster,
                   const FT_Raster_Params*  params )
  {
    const FT_Outline*  outline    = (const FT_Outline*)params->source;
    const FT_Bitmap*   target_map = params->target;
    black_PWorker      worker;


    if ( !raster || !raster->buffer || !raster->buffer_size )
      return FT_THROW( Not_Ini );

    if ( !outline )
      return FT_THROW( Invalid );

    /* return immediately if the outline is empty */
    if ( outline->n_points == 0 || outline->n_contours <= 0 )
      return Raster_Err_None;

    if ( !outline->contours || !outline->points )
      return FT_THROW( Invalid );

    if ( outline->n_points !=
           outline->contours[outline->n_contours - 1] + 1 )
      return FT_THROW( Invalid );

    worker = raster->worker;

    /* this version of the raster does not support direct rendering, sorry */
    if ( params->flags & FT_RASTER_FLAG_DIRECT )
      return FT_THROW( Unsupported );

    if ( !target_map )
      return FT_THROW( Invalid );

    /* nothing to do */
    if ( !target_map->width || !target_map->rows )
      return Raster_Err_None;

    if ( !target_map->buffer )
      return FT_THROW( Invalid );

    ras.outline = *outline;
    ras.target  = *target_map;

    worker->buff       = (PLong) raster->buffer;
    worker->sizeBuff   = worker->buff +
                           raster->buffer_size / sizeof ( Long );
#ifdef FT_RASTER_OPTION_ANTI_ALIASING
    worker->grays      = raster->grays;
    worker->gray_width = raster->gray_width;

    FT_MEM_ZERO( worker->gray_lines, worker->gray_width * 2 );
#endif

    return ( params->flags & FT_RASTER_FLAG_AA )
           ? Render_Gray_Glyph( RAS_VAR )
           : Render_Glyph( RAS_VAR );
  }

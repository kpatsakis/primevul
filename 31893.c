  sfnt_done_face( TT_Face  face )
  {
    FT_Memory     memory;
    SFNT_Service  sfnt;


    if ( !face )
      return;

    memory = face->root.memory;
    sfnt   = (SFNT_Service)face->sfnt;

    if ( sfnt )
    {
      /* destroy the postscript names table if it is loaded */
      if ( sfnt->free_psnames )
        sfnt->free_psnames( face );

      /* destroy the embedded bitmaps table if it is loaded */
      if ( sfnt->free_eblc )
        sfnt->free_eblc( face );
    }

#ifdef TT_CONFIG_OPTION_BDF
    /* freeing the embedded BDF properties */
    tt_face_free_bdf_props( face );
#endif

    /* freeing the kerning table */
    tt_face_done_kern( face );

    /* freeing the collection table */
    FT_FREE( face->ttc_header.offsets );
    face->ttc_header.count = 0;

    /* freeing table directory */
    FT_FREE( face->dir_tables );
    face->num_tables = 0;

    {
      FT_Stream  stream = FT_FACE_STREAM( face );


      /* simply release the 'cmap' table frame */
      FT_FRAME_RELEASE( face->cmap_table );
      face->cmap_size = 0;
    }

    face->horz_metrics_size = 0;
    face->vert_metrics_size = 0;

    /* freeing vertical metrics, if any */
    if ( face->vertical_info )
    {
      FT_FREE( face->vertical.long_metrics  );
      FT_FREE( face->vertical.short_metrics );
      face->vertical_info = 0;
    }

    /* freeing the gasp table */
    FT_FREE( face->gasp.gaspRanges );
    face->gasp.numRanges = 0;

    /* freeing the name table */
    if ( sfnt )
      sfnt->free_name( face );

    /* freeing family and style name */
    FT_FREE( face->root.family_name );
    FT_FREE( face->root.style_name );

    /* freeing sbit size table */
    FT_FREE( face->root.available_sizes );
    FT_FREE( face->sbit_strike_map );
    face->root.num_fixed_sizes = 0;

    FT_FREE( face->postscript_name );

    face->sfnt = NULL;
  }

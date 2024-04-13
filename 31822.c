  tt_cmap14_done( TT_CMap14  cmap )
  {
    FT_Memory  memory = cmap->memory;


    cmap->max_results = 0;
    if ( memory != NULL && cmap->results != NULL )
      FT_FREE( cmap->results );
  }

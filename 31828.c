  tt_cmap14_init( TT_CMap14  cmap,
                  FT_Byte*   table )
  {
    cmap->cmap.data = table;

    table               += 6;
    cmap->num_selectors  = FT_PEEK_ULONG( table );
    cmap->max_results    = 0;
    cmap->results        = NULL;

    return FT_Err_Ok;
  }

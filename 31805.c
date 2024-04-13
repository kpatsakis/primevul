  tt_cmap12_get_info( TT_CMap       cmap,
                      TT_CMapInfo  *cmap_info )
  {
    FT_Byte*  p = cmap->data + 8;


    cmap_info->format   = 12;
    cmap_info->language = (FT_ULong)TT_PEEK_ULONG( p );

    return FT_Err_Ok;
  }

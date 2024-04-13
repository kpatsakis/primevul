  tt_cmap4_init( TT_CMap4  cmap,
                 FT_Byte*  table )
  {
    FT_Byte*  p;


    cmap->cmap.data    = table;

    p                  = table + 6;
    cmap->num_ranges   = FT_PEEK_USHORT( p ) >> 1;
    cmap->cur_charcode = (FT_UInt32)0xFFFFFFFFUL;
    cmap->cur_gindex   = 0;

    return FT_Err_Ok;
  }

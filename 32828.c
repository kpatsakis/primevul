  FT_Get_First_Char( FT_Face   face,
                     FT_UInt  *agindex )
  {
    FT_ULong  result = 0;
    FT_UInt   gindex = 0;


    if ( face && face->charmap && face->num_glyphs )
    {
      gindex = FT_Get_Char_Index( face, 0 );
      if ( gindex == 0 || gindex >= (FT_UInt)face->num_glyphs )
        result = FT_Get_Next_Char( face, 0, &gindex );
    }

    if ( agindex )
      *agindex = gindex;

    return result;
  }

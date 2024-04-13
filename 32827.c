  FT_Get_Charmap_Index( FT_CharMap  charmap )
  {
    FT_Int  i;


    if ( !charmap || !charmap->face )
      return -1;

    for ( i = 0; i < charmap->face->num_charmaps; i++ )
      if ( charmap->face->charmaps[i] == charmap )
        break;

    FT_ASSERT( i < charmap->face->num_charmaps );

    return i;
  }

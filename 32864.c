  ft_lookup_PS_in_sfnt_stream( FT_Stream  stream,
                               FT_Long    face_index,
                               FT_ULong*  offset,
                               FT_ULong*  length,
                               FT_Bool*   is_sfnt_cid )
  {
    FT_Error   error;
    FT_UShort  numTables;
    FT_Long    pstable_index;
    FT_ULong   tag;
    int        i;


    *offset = 0;
    *length = 0;
    *is_sfnt_cid = FALSE;

    /* TODO: support for sfnt-wrapped PS/CID in TTC format */

    /* version check for 'typ1' (should be ignored?) */
    if ( FT_READ_ULONG( tag ) )
      return error;
    if ( tag != TTAG_typ1 )
      return FT_Err_Unknown_File_Format;

    if ( FT_READ_USHORT( numTables ) )
      return error;
    if ( FT_STREAM_SKIP( 2 * 3 ) ) /* skip binary search header */
      return error;

    pstable_index = -1;
    *is_sfnt_cid  = FALSE;

    for ( i = 0; i < numTables; i++ )
    {
      if ( FT_READ_ULONG( tag )     || FT_STREAM_SKIP( 4 )      ||
           FT_READ_ULONG( *offset ) || FT_READ_ULONG( *length ) )
        return error;

      if ( tag == TTAG_CID )
      {
        pstable_index++;
        *offset += 22;
        *length -= 22;
        *is_sfnt_cid = TRUE;
        if ( face_index < 0 )
          return FT_Err_Ok;
      }
      else if ( tag == TTAG_TYP1 )
      {
        pstable_index++;
        *offset += 24;
        *length -= 24;
        *is_sfnt_cid = FALSE;
        if ( face_index < 0 )
          return FT_Err_Ok;
      }
      if ( face_index >= 0 && pstable_index == face_index )
        return FT_Err_Ok;
    }
    return FT_Err_Table_Missing;
  }

static HB_Error  Load_MarkArray ( HB_MarkArray*  ma,
				  HB_Stream       stream )
{
  HB_Error  error;

  HB_UShort        n, m, count;
  HB_UInt         cur_offset, new_offset, base_offset;

  HB_MarkRecord*  mr;


  base_offset = FILE_Pos();

  if ( ACCESS_Frame( 2L ) )
    return error;

  count = ma->MarkCount = GET_UShort();

  FORGET_Frame();

  ma->MarkRecord = NULL;

  if ( ALLOC_ARRAY( ma->MarkRecord, count, HB_MarkRecord ) )
    return error;

  mr = ma->MarkRecord;

  for ( n = 0; n < count; n++ )
  {
    if ( ACCESS_Frame( 4L ) )
      goto Fail;

    mr[n].Class = GET_UShort();
    new_offset  = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
	 ( error = Load_Anchor( &mr[n].MarkAnchor, stream ) ) != HB_Err_Ok )
      goto Fail;
    (void)FILE_Seek( cur_offset );
  }

  return HB_Err_Ok;

Fail:
  for ( m = 0; m < n; m++ )
    Free_Anchor( &mr[m].MarkAnchor );

  FREE( mr );
  return error;
}

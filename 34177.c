static HB_Error  Load_CursivePos( HB_GPOS_SubTable* st,
				  HB_Stream        stream )
{
  HB_Error  error;
  HB_CursivePos*  cp = &st->cursive;

  HB_UShort             n, m, count;
  HB_UInt              cur_offset, new_offset, base_offset;

  HB_EntryExitRecord*  eer;


  base_offset = FILE_Pos();

  if ( ACCESS_Frame( 4L ) )
    return error;

  cp->PosFormat = GET_UShort();
  new_offset    = GET_UShort() + base_offset;

  FORGET_Frame();

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = _HB_OPEN_Load_Coverage( &cp->Coverage, stream ) ) != HB_Err_Ok )
    return error;
  (void)FILE_Seek( cur_offset );

  if ( ACCESS_Frame( 2L ) )
    goto Fail2;

  count = cp->EntryExitCount = GET_UShort();

  FORGET_Frame();

  cp->EntryExitRecord = NULL;

  if ( ALLOC_ARRAY( cp->EntryExitRecord, count, HB_EntryExitRecord ) )
    goto Fail2;

  eer = cp->EntryExitRecord;

  for ( n = 0; n < count; n++ )
  {
    HB_UInt entry_offset;

    if ( ACCESS_Frame( 2L ) )
      return error;

    entry_offset = new_offset = GET_UShort();

    FORGET_Frame();

    if ( new_offset )
    {
      new_offset += base_offset;

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
	   ( error = Load_Anchor( &eer[n].EntryAnchor,
				  stream ) ) != HB_Err_Ok )
	goto Fail1;
      (void)FILE_Seek( cur_offset );
    }
    else
      eer[n].EntryAnchor.PosFormat   = 0;

    if ( ACCESS_Frame( 2L ) )
      return error;

    new_offset = GET_UShort();

    FORGET_Frame();

    if ( new_offset )
    {
      new_offset += base_offset;

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
	   ( error = Load_Anchor( &eer[n].ExitAnchor,
				  stream ) ) != HB_Err_Ok )
      {
	if ( entry_offset )
	  Free_Anchor( &eer[n].EntryAnchor );
	goto Fail1;
      }
      (void)FILE_Seek( cur_offset );
    }
    else
      eer[n].ExitAnchor.PosFormat   = 0;
  }

  return HB_Err_Ok;

Fail1:
  for ( m = 0; m < n; m++ )
  {
    Free_Anchor( &eer[m].EntryAnchor );
    Free_Anchor( &eer[m].ExitAnchor );
  }

  FREE( eer );

Fail2:
  _HB_OPEN_Free_Coverage( &cp->Coverage );
  return error;
}

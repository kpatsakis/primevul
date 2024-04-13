static HB_Error  Load_ContextPos3( HB_ContextPosFormat3*  cpf3,
				   HB_Stream               stream )
{
  HB_Error  error;

  HB_UShort             n, count;
  HB_UInt              cur_offset, new_offset, base_offset;

  HB_Coverage*         c;
  HB_PosLookupRecord*  plr;


  base_offset = FILE_Pos() - 2L;

  if ( ACCESS_Frame( 4L ) )
    return error;

  cpf3->GlyphCount = GET_UShort();
  cpf3->PosCount   = GET_UShort();

  FORGET_Frame();

  cpf3->Coverage = NULL;

  count = cpf3->GlyphCount;

  if ( ALLOC_ARRAY( cpf3->Coverage, count, HB_Coverage ) )
    return error;

  c = cpf3->Coverage;

  for ( n = 0; n < count; n++ )
  {
    if ( ACCESS_Frame( 2L ) )
      goto Fail2;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
	 ( error = _HB_OPEN_Load_Coverage( &c[n], stream ) ) != HB_Err_Ok )
      goto Fail2;
    (void)FILE_Seek( cur_offset );
  }

  cpf3->PosLookupRecord = NULL;

  count = cpf3->PosCount;

  if ( ALLOC_ARRAY( cpf3->PosLookupRecord, count, HB_PosLookupRecord ) )
    goto Fail2;

  plr = cpf3->PosLookupRecord;

  if ( ACCESS_Frame( count * 4L ) )
    goto Fail1;

  for ( n = 0; n < count; n++ )
  {
    plr[n].SequenceIndex   = GET_UShort();
    plr[n].LookupListIndex = GET_UShort();
  }

  FORGET_Frame();

  return HB_Err_Ok;

Fail1:
  FREE( plr );

Fail2:
  for ( n = 0; n < count; n++ )
    _HB_OPEN_Free_Coverage( &c[n] );

  FREE( c );
  return error;
}

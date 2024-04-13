static HB_Error  Load_ContextPos2( HB_ContextPosFormat2*  cpf2,
				   HB_Stream               stream )
{
  HB_Error  error;

  HB_UShort         n, m, count;
  HB_UInt          cur_offset, new_offset, base_offset;

  HB_PosClassSet*  pcs;


  base_offset = FILE_Pos() - 2;

  if ( ACCESS_Frame( 2L ) )
    return error;

  new_offset = GET_UShort() + base_offset;

  FORGET_Frame();

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = _HB_OPEN_Load_Coverage( &cpf2->Coverage, stream ) ) != HB_Err_Ok )
    return error;
  (void)FILE_Seek( cur_offset );

  if ( ACCESS_Frame( 4L ) )
    goto Fail3;

  new_offset = GET_UShort() + base_offset;

  /* `PosClassSetCount' is the upper limit for class values, thus we
     read it now to make an additional safety check.                 */

  count = cpf2->PosClassSetCount = GET_UShort();

  FORGET_Frame();

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = _HB_OPEN_Load_ClassDefinition( &cpf2->ClassDef, count,
				       stream ) ) != HB_Err_Ok )
    goto Fail3;
  (void)FILE_Seek( cur_offset );

  cpf2->PosClassSet      = NULL;
  cpf2->MaxContextLength = 0;

  if ( ALLOC_ARRAY( cpf2->PosClassSet, count, HB_PosClassSet ) )
    goto Fail2;

  pcs = cpf2->PosClassSet;

  for ( n = 0; n < count; n++ )
  {
    if ( ACCESS_Frame( 2L ) )
      goto Fail1;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    if ( new_offset != base_offset )      /* not a NULL offset */
    {
      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
	   ( error = Load_PosClassSet( cpf2, &pcs[n],
				       stream ) ) != HB_Err_Ok )
	goto Fail1;
      (void)FILE_Seek( cur_offset );
    }
    else
    {
      /* we create a PosClassSet table with no entries */

      cpf2->PosClassSet[n].PosClassRuleCount = 0;
      cpf2->PosClassSet[n].PosClassRule      = NULL;
    }
  }

  return HB_Err_Ok;

Fail1:
  for ( m = 0; m < n; n++ )
    Free_PosClassSet( &pcs[m] );

  FREE( pcs );

Fail2:
  _HB_OPEN_Free_ClassDefinition( &cpf2->ClassDef );

Fail3:
  _HB_OPEN_Free_Coverage( &cpf2->Coverage );
  return error;
}

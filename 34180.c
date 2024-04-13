static HB_Error  Load_Mark2Array( HB_Mark2Array*  m2a,
				  HB_UShort        num_classes,
				  HB_Stream        stream )
{
  HB_Error  error;

  HB_UShort        m, n, count;
  HB_UInt          cur_offset, new_offset, base_offset;

  HB_Mark2Record  *m2r;
  HB_Anchor       *m2an, *m2ans;


  base_offset = FILE_Pos();

  if ( ACCESS_Frame( 2L ) )
    return error;

  count = m2a->Mark2Count = GET_UShort();

  FORGET_Frame();

  m2a->Mark2Record = NULL;

  if ( ALLOC_ARRAY( m2a->Mark2Record, count, HB_Mark2Record ) )
    return error;

  m2r = m2a->Mark2Record;

  m2ans = NULL;

  if ( ALLOC_ARRAY( m2ans, count * num_classes, HB_Anchor ) )
    goto Fail;

  for ( m = 0; m < count; m++ )
  {
    m2an = m2r[m].Mark2Anchor = m2ans + m * num_classes;

    for ( n = 0; n < num_classes; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
	goto Fail;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      if (new_offset == base_offset) {
        /* Anchor table not provided.  Skip loading.
	 * Some versions of FreeSans hit this. */
        m2an[n].PosFormat = 0;
	continue;
      }

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
	   ( error = Load_Anchor( &m2an[n], stream ) ) != HB_Err_Ok )
	goto Fail;
      (void)FILE_Seek( cur_offset );
    }
  }

  return HB_Err_Ok;

Fail:
  FREE( m2ans );
  FREE( m2r );
  return error;
}

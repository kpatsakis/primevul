HB_Error  HB_Load_GPOS_Table( HB_Stream stream, 
			      HB_GPOSHeader** retptr,
			      HB_GDEFHeader*  gdef,
			      HB_Stream       gdefStream )
{
  HB_UInt         cur_offset, new_offset, base_offset;

  HB_GPOSHeader*  gpos;

  HB_Error   error;


  if ( !retptr )
    return ERR(HB_Err_Invalid_Argument);

  if ( GOTO_Table( TTAG_GPOS ) )
    return error;

  base_offset = FILE_Pos();

  if ( ALLOC ( gpos, sizeof( *gpos ) ) )
    return error;

#ifdef HB_SUPPORT_MULTIPLE_MASTER
  gpos->mmfunc = default_mmfunc;
#endif

  /* skip version */

  if ( FILE_Seek( base_offset + 4L ) ||
       ACCESS_Frame( 2L ) )
    goto Fail4;

  new_offset = GET_UShort() + base_offset;

  FORGET_Frame();

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = _HB_OPEN_Load_ScriptList( &gpos->ScriptList,
				  stream ) ) != HB_Err_Ok )
    goto Fail4;
  (void)FILE_Seek( cur_offset );

  if ( ACCESS_Frame( 2L ) )
    goto Fail3;

  new_offset = GET_UShort() + base_offset;

  FORGET_Frame();

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = _HB_OPEN_Load_FeatureList( &gpos->FeatureList,
				   stream ) ) != HB_Err_Ok )
    goto Fail3;
  (void)FILE_Seek( cur_offset );

  if ( ACCESS_Frame( 2L ) )
    goto Fail2;

  new_offset = GET_UShort() + base_offset;

  FORGET_Frame();

  cur_offset = FILE_Pos();
  if ( FILE_Seek( new_offset ) ||
       ( error = _HB_OPEN_Load_LookupList( &gpos->LookupList,
				  stream, HB_Type_GPOS ) ) != HB_Err_Ok )
    goto Fail2;

  gpos->gdef = gdef;      /* can be NULL */

  if ( ( error =  _HB_GDEF_LoadMarkAttachClassDef_From_LookupFlags( gdef, gdefStream,
								     gpos->LookupList.Lookup,
								     gpos->LookupList.LookupCount ) ) )
    goto Fail1;

  *retptr = gpos;

  return HB_Err_Ok;

Fail1:
  _HB_OPEN_Free_LookupList( &gpos->LookupList, HB_Type_GPOS );

Fail2:
  _HB_OPEN_Free_FeatureList( &gpos->FeatureList );

Fail3:
  _HB_OPEN_Free_ScriptList( &gpos->ScriptList );

Fail4:
  FREE( gpos );

  return error;
}

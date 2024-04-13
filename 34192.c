static HB_Error  Load_ValueRecord( HB_ValueRecord*  vr,
				   HB_UShort         format,
				   HB_UInt          base_offset,
				   HB_Stream         stream )
{
  HB_Error  error;

  HB_UInt cur_offset, new_offset;


  if ( format & HB_GPOS_FORMAT_HAVE_X_PLACEMENT )
  {
    if ( ACCESS_Frame( 2L ) )
      return error;

    vr->XPlacement = GET_Short();

    FORGET_Frame();
  }
  else
    vr->XPlacement = 0;

  if ( format & HB_GPOS_FORMAT_HAVE_Y_PLACEMENT )
  {
    if ( ACCESS_Frame( 2L ) )
      return error;

    vr->YPlacement = GET_Short();

    FORGET_Frame();
  }
  else
    vr->YPlacement = 0;

  if ( format & HB_GPOS_FORMAT_HAVE_X_ADVANCE )
  {
    if ( ACCESS_Frame( 2L ) )
      return error;

    vr->XAdvance = GET_Short();

    FORGET_Frame();
  }
  else
    vr->XAdvance = 0;

  if ( format & HB_GPOS_FORMAT_HAVE_Y_ADVANCE )
  {
    if ( ACCESS_Frame( 2L ) )
      return error;

    vr->YAdvance = GET_Short();

    FORGET_Frame();
  }
  else
    vr->YAdvance = 0;

  if ( format & HB_GPOS_FORMAT_HAVE_DEVICE_TABLES )
  {
    if ( ALLOC_ARRAY( vr->DeviceTables, 4, HB_Device ) )
      return error;
    vr->DeviceTables[VR_X_ADVANCE_DEVICE] = 0;
    vr->DeviceTables[VR_Y_ADVANCE_DEVICE] = 0;
    vr->DeviceTables[VR_X_PLACEMENT_DEVICE] = 0;
    vr->DeviceTables[VR_Y_PLACEMENT_DEVICE] = 0;
  }
  else
  {
    vr->DeviceTables = 0;
  }

  if ( format & HB_GPOS_FORMAT_HAVE_X_PLACEMENT_DEVICE )
  {
    if ( ACCESS_Frame( 2L ) )
      goto Fail4;

    new_offset = GET_UShort();

    FORGET_Frame();

    if ( new_offset )
    {
      new_offset += base_offset;

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
	   ( error = _HB_OPEN_Load_Device( &vr->DeviceTables[VR_X_PLACEMENT_DEVICE],
				  stream ) ) != HB_Err_Ok )
       goto Fail4;
      (void)FILE_Seek( cur_offset );
    }
  }

  if ( format & HB_GPOS_FORMAT_HAVE_Y_PLACEMENT_DEVICE )
  {
    if ( ACCESS_Frame( 2L ) )
      goto Fail3;

    new_offset = GET_UShort();

    FORGET_Frame();

    if ( new_offset )
    {
      new_offset += base_offset;

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
	   ( error = _HB_OPEN_Load_Device( &vr->DeviceTables[VR_Y_PLACEMENT_DEVICE],
				  stream ) ) != HB_Err_Ok )
	goto Fail3;
      (void)FILE_Seek( cur_offset );
    }
  }

  if ( format & HB_GPOS_FORMAT_HAVE_X_ADVANCE_DEVICE )
  {
    if ( ACCESS_Frame( 2L ) )
      goto Fail2;

    new_offset = GET_UShort();

    FORGET_Frame();

    if ( new_offset )
    {
      new_offset += base_offset;

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
	   ( error = _HB_OPEN_Load_Device( &vr->DeviceTables[VR_X_ADVANCE_DEVICE],
				  stream ) ) != HB_Err_Ok )
	goto Fail2;
      (void)FILE_Seek( cur_offset );
    }
  }

  if ( format & HB_GPOS_FORMAT_HAVE_Y_ADVANCE_DEVICE )
  {
    if ( ACCESS_Frame( 2L ) )
      goto Fail1;

    new_offset = GET_UShort();

    FORGET_Frame();

    if ( new_offset )
    {
      new_offset += base_offset;

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
	   ( error = _HB_OPEN_Load_Device( &vr->DeviceTables[VR_Y_ADVANCE_DEVICE],
				  stream ) ) != HB_Err_Ok )
	goto Fail1;
      (void)FILE_Seek( cur_offset );
    }
  }

  if ( format & HB_GPOS_FORMAT_HAVE_X_ID_PLACEMENT )
  {
    if ( ACCESS_Frame( 2L ) )
      goto Fail1;

#ifdef HB_SUPPORT_MULTIPLE_MASTER
    vr->XIdPlacement = GET_UShort();
#else
    (void) GET_UShort();
#endif

    FORGET_Frame();
  }
#ifdef HB_SUPPORT_MULTIPLE_MASTER
  else
    vr->XIdPlacement = 0;
#endif

  if ( format & HB_GPOS_FORMAT_HAVE_Y_ID_PLACEMENT )
  {
    if ( ACCESS_Frame( 2L ) )
      goto Fail1;

#ifdef HB_SUPPORT_MULTIPLE_MASTER
    vr->YIdPlacement = GET_UShort();
#else
    (void) GET_UShort();
#endif

    FORGET_Frame();
  }
#ifdef HB_SUPPORT_MULTIPLE_MASTER
  else
    vr->YIdPlacement = 0;
#endif

  if ( format & HB_GPOS_FORMAT_HAVE_X_ID_ADVANCE )
  {
    if ( ACCESS_Frame( 2L ) )
      goto Fail1;

#ifdef HB_SUPPORT_MULTIPLE_MASTER
    vr->XIdAdvance = GET_UShort();
#else
    (void) GET_UShort();
#endif

    FORGET_Frame();
  }
#ifdef HB_SUPPORT_MULTIPLE_MASTER
  else
    vr->XIdAdvance = 0;
#endif

  if ( format & HB_GPOS_FORMAT_HAVE_Y_ID_ADVANCE )
  {
    if ( ACCESS_Frame( 2L ) )
      goto Fail1;

#ifdef HB_SUPPORT_MULTIPLE_MASTER
    vr->YIdAdvance = GET_UShort();
#else
    (void) GET_UShort();
#endif

    FORGET_Frame();
  }
#ifdef HB_SUPPORT_MULTIPLE_MASTER
  else
    vr->YIdAdvance = 0;
#endif

  return HB_Err_Ok;

Fail1:
  if ( vr->DeviceTables )
    _HB_OPEN_Free_Device( vr->DeviceTables[VR_Y_ADVANCE_DEVICE] );

Fail2:
  if ( vr->DeviceTables )
    _HB_OPEN_Free_Device( vr->DeviceTables[VR_X_ADVANCE_DEVICE] );

Fail3:
  if ( vr->DeviceTables )
    _HB_OPEN_Free_Device( vr->DeviceTables[VR_Y_PLACEMENT_DEVICE] );

Fail4:
  FREE( vr->DeviceTables );
  return error;
}

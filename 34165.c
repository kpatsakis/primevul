static HB_Error  Load_Anchor( HB_Anchor*  an,
			      HB_Stream    stream )
{
  HB_Error  error;

  HB_UInt cur_offset, new_offset, base_offset;


  base_offset = FILE_Pos();

  if ( ACCESS_Frame( 2L ) )
    return error;

  an->PosFormat = GET_UShort();

  FORGET_Frame();

  switch ( an->PosFormat )
  {
  case 1:
    if ( ACCESS_Frame( 4L ) )
      return error;

    an->af.af1.XCoordinate = GET_Short();
    an->af.af1.YCoordinate = GET_Short();

    FORGET_Frame();
    break;

  case 2:
    if ( ACCESS_Frame( 6L ) )
      return error;

    an->af.af2.XCoordinate = GET_Short();
    an->af.af2.YCoordinate = GET_Short();
    an->af.af2.AnchorPoint = GET_UShort();

    FORGET_Frame();
    break;

  case 3:
    if ( ACCESS_Frame( 6L ) )
      return error;

    an->af.af3.XCoordinate = GET_Short();
    an->af.af3.YCoordinate = GET_Short();

    new_offset = GET_UShort();

    FORGET_Frame();

    if ( new_offset )
    {
      if ( ALLOC_ARRAY( an->af.af3.DeviceTables, 2, HB_Device ) )
        return error;

      an->af.af3.DeviceTables[AF3_X_DEVICE_TABLE] = 0;
      an->af.af3.DeviceTables[AF3_Y_DEVICE_TABLE] = 0;

      new_offset += base_offset;

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
	   ( error = _HB_OPEN_Load_Device( &an->af.af3.DeviceTables[AF3_X_DEVICE_TABLE],
				  stream ) ) != HB_Err_Ok )
	goto Fail2;
      (void)FILE_Seek( cur_offset );
    }

    if ( ACCESS_Frame( 2L ) )
      goto Fail;

    new_offset = GET_UShort();

    FORGET_Frame();

    if ( new_offset )
    {
      if ( !an->af.af3.DeviceTables )
      {
        if ( ALLOC_ARRAY( an->af.af3.DeviceTables, 2, HB_Device ) )
          return error;

        an->af.af3.DeviceTables[AF3_X_DEVICE_TABLE] = 0;
        an->af.af3.DeviceTables[AF3_Y_DEVICE_TABLE] = 0;
      }

      new_offset += base_offset;

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
	   ( error = _HB_OPEN_Load_Device( &an->af.af3.DeviceTables[AF3_Y_DEVICE_TABLE],
				  stream ) ) != HB_Err_Ok )
	goto Fail;
      (void)FILE_Seek( cur_offset );
    }
    break;

  case 4:
    if ( ACCESS_Frame( 4L ) )
      return error;

#ifdef HB_SUPPORT_MULTIPLE_MASTER
    an->af.af4.XIdAnchor = GET_UShort();
    an->af.af4.YIdAnchor = GET_UShort();
#else
    (void) GET_UShort();
    (void) GET_UShort();
#endif

    FORGET_Frame();
    break;

  default:
    return ERR(HB_Err_Invalid_SubTable_Format);
  }

  return HB_Err_Ok;

Fail:
  if ( an->af.af3.DeviceTables )
    _HB_OPEN_Free_Device( an->af.af3.DeviceTables[AF3_X_DEVICE_TABLE] );

Fail2:
  FREE( an->af.af3.DeviceTables );
  return error;
}

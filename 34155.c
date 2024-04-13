static HB_Error  Get_ValueRecord( GPOS_Instance*    gpi,
				  HB_ValueRecord*  vr,
				  HB_UShort         format,
				  HB_Position      gd )
{
  HB_Short         pixel_value;
  HB_Error         error = HB_Err_Ok;
#ifdef HB_SUPPORT_MULTIPLE_MASTER
  HB_GPOSHeader*  gpos = gpi->gpos;
  HB_Fixed           value;
#endif

  HB_UShort  x_ppem, y_ppem;
  HB_16Dot16   x_scale, y_scale;


  if ( !format )
    return HB_Err_Ok;

  x_ppem  = gpi->font->x_ppem;
  y_ppem  = gpi->font->y_ppem;
  x_scale = gpi->font->x_scale;
  y_scale = gpi->font->y_scale;

  /* design units -> fractional pixel */

  if ( format & HB_GPOS_FORMAT_HAVE_X_PLACEMENT )
    gd->x_pos += x_scale * vr->XPlacement / 0x10000;
  if ( format & HB_GPOS_FORMAT_HAVE_Y_PLACEMENT )
    gd->y_pos += y_scale * vr->YPlacement / 0x10000;
  if ( format & HB_GPOS_FORMAT_HAVE_X_ADVANCE )
    gd->x_advance += x_scale * vr->XAdvance / 0x10000;
  if ( format & HB_GPOS_FORMAT_HAVE_Y_ADVANCE )
    gd->y_advance += y_scale * vr->YAdvance / 0x10000;

  if ( !gpi->dvi )
  {
    /* pixel -> fractional pixel */

    if ( format & HB_GPOS_FORMAT_HAVE_X_PLACEMENT_DEVICE )
    {
      _HB_OPEN_Get_Device( vr->DeviceTables[VR_X_PLACEMENT_DEVICE], x_ppem, &pixel_value );
      gd->x_pos += pixel_value << 6;
    }
    if ( format & HB_GPOS_FORMAT_HAVE_Y_PLACEMENT_DEVICE )
    {
      _HB_OPEN_Get_Device( vr->DeviceTables[VR_Y_PLACEMENT_DEVICE], y_ppem, &pixel_value );
      gd->y_pos += pixel_value << 6;
    }
    if ( format & HB_GPOS_FORMAT_HAVE_X_ADVANCE_DEVICE )
    {
      _HB_OPEN_Get_Device( vr->DeviceTables[VR_X_ADVANCE_DEVICE], x_ppem, &pixel_value );
      gd->x_advance += pixel_value << 6;
    }
    if ( format & HB_GPOS_FORMAT_HAVE_Y_ADVANCE_DEVICE )
    {
      _HB_OPEN_Get_Device( vr->DeviceTables[VR_Y_ADVANCE_DEVICE], y_ppem, &pixel_value );
      gd->y_advance += pixel_value << 6;
    }
  }

#ifdef HB_SUPPORT_MULTIPLE_MASTER
  /* values returned from mmfunc() are already in fractional pixels */

  if ( format & HB_GPOS_FORMAT_HAVE_X_ID_PLACEMENT )
  {
    error = (gpos->mmfunc)( gpi->font, vr->XIdPlacement,
			    &value, gpos->data );
    if ( error )
      return error;
    gd->x_pos += value;
  }
  if ( format & HB_GPOS_FORMAT_HAVE_Y_ID_PLACEMENT )
  {
    error = (gpos->mmfunc)( gpi->font, vr->YIdPlacement,
			    &value, gpos->data );
    if ( error )
      return error;
    gd->y_pos += value;
  }
  if ( format & HB_GPOS_FORMAT_HAVE_X_ID_ADVANCE )
  {
    error = (gpos->mmfunc)( gpi->font, vr->XIdAdvance,
			    &value, gpos->data );
    if ( error )
      return error;
    gd->x_advance += value;
  }
  if ( format & HB_GPOS_FORMAT_HAVE_Y_ID_ADVANCE )
  {
    error = (gpos->mmfunc)( gpi->font, vr->YIdAdvance,
			    &value, gpos->data );
    if ( error )
      return error;
    gd->y_advance += value;
  }
#endif

  return error;
}

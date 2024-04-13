static void  Free_ValueRecord( HB_ValueRecord*  vr,
			       HB_UShort         format )
{
  if ( format & HB_GPOS_FORMAT_HAVE_Y_ADVANCE_DEVICE )
    _HB_OPEN_Free_Device( vr->DeviceTables[VR_Y_ADVANCE_DEVICE] );
  if ( format & HB_GPOS_FORMAT_HAVE_X_ADVANCE_DEVICE )
    _HB_OPEN_Free_Device( vr->DeviceTables[VR_X_ADVANCE_DEVICE] );
  if ( format & HB_GPOS_FORMAT_HAVE_Y_PLACEMENT_DEVICE )
    _HB_OPEN_Free_Device( vr->DeviceTables[VR_Y_PLACEMENT_DEVICE] );
  if ( format & HB_GPOS_FORMAT_HAVE_X_PLACEMENT_DEVICE )
    _HB_OPEN_Free_Device( vr->DeviceTables[VR_X_PLACEMENT_DEVICE] );
  FREE( vr->DeviceTables );
}

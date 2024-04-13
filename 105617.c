update_info_drive_can_spindown (Device *device)
{
  gboolean drive_can_spindown;

  /* Right now we only know how to spin down ATA devices (including those USB devices
   * that can do ATA SMART)
   *
   * This would probably also work for SCSI devices (since the helper is doing SCSI
   * STOP (which translated in libata to ATA's STANDBY IMMEDIATE) - but that needs
   * testing...
   */
  drive_can_spindown = FALSE;
  if (g_strcmp0 (device->priv->drive_connection_interface, "ata") == 0 || device->priv->drive_ata_smart_is_available)
    {
      drive_can_spindown = TRUE;
    }
  if (g_udev_device_has_property (device->priv->d, "ID_DRIVE_CAN_SPINDOWN"))
    {
      drive_can_spindown = g_udev_device_get_property_as_boolean (device->priv->d, "ID_DRIVE_CAN_SPINDOWN");
    }
  device_set_drive_can_spindown (device, drive_can_spindown);

  return TRUE;
}

update_info_presentation (Device *device)
{
  gboolean hide;
  gboolean nopolicy;

  hide = FALSE;
  if (g_udev_device_has_property (device->priv->d, "UDISKS_PRESENTATION_HIDE"))
    hide = g_udev_device_get_property_as_boolean (device->priv->d, "UDISKS_PRESENTATION_HIDE");
  device_set_device_presentation_hide (device, hide);

  nopolicy = FALSE;
  if (g_udev_device_has_property (device->priv->d, "UDISKS_PRESENTATION_NOPOLICY"))
    nopolicy = g_udev_device_get_property_as_boolean (device->priv->d, "UDISKS_PRESENTATION_NOPOLICY");
  device_set_device_presentation_nopolicy (device, nopolicy);

  device_set_device_presentation_name (device, g_udev_device_get_property (device->priv->d, "UDISKS_PRESENTATION_NAME"));

  device_set_device_presentation_icon_name (device, g_udev_device_get_property (device->priv->d,
                                                                                "UDISKS_PRESENTATION_ICON_NAME"));

  return TRUE;
}

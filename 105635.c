update_info_media_detection (Device *device)
{
  gboolean detected;
  gboolean polling;
  gboolean inhibitable;
  gboolean inhibited;

  detected = FALSE;
  polling = FALSE;
  inhibitable = FALSE;
  inhibited = FALSE;

  if (device->priv->device_is_removable)
    {
      guint64 evt_media_change;
      GUdevDevice *parent;

      evt_media_change = sysfs_get_uint64 (device->priv->native_path, "../../evt_media_change");
      if (evt_media_change & 1)
        {
          /* SATA AN capabable drive */

          polling = FALSE;
          detected = TRUE;
          goto determined;
        }

      parent = g_udev_device_get_parent_with_subsystem (device->priv->d, "platform", NULL);
      if (parent != NULL)
        {
          /* never poll PC floppy drives, they are noisy (fdo #22149) */
          if (g_str_has_prefix (g_udev_device_get_name (parent), "floppy."))
            {
              g_object_unref (parent);
              goto determined;
            }
          g_object_unref (parent);
        }

      /* assume the device needs polling */
      polling = TRUE;
      inhibitable = TRUE;

      /* custom udev rules might want to disable polling for known-broken
       * devices (fdo #26508) */
      if (g_udev_device_has_property (device->priv->d, "UDISKS_DISABLE_POLLING") &&
	  g_udev_device_get_property_as_boolean (device->priv->d, "UDISKS_DISABLE_POLLING"))
	polling = FALSE;

      if (device->priv->polling_inhibitors != NULL || daemon_local_has_polling_inhibitors (device->priv->daemon))
        {

          detected = FALSE;
          inhibited = TRUE;
        }
      else
        {
          detected = TRUE;
          inhibited = FALSE;
        }
    }

 determined:
  device_set_device_is_media_change_detected (device, detected);
  device_set_device_is_media_change_detection_polling (device, polling);
  device_set_device_is_media_change_detection_inhibitable (device, inhibitable);
  device_set_device_is_media_change_detection_inhibited (device, inhibited);

  return TRUE;
}

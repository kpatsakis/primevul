update_info_id (Device *device)
{
  gchar *decoded_string;
  const gchar *partition_scheme;
  gint partition_type;

  partition_scheme = g_udev_device_get_property (device->priv->d, "UDISKS_PARTITION_SCHEME");
  partition_type = g_udev_device_get_property_as_int (device->priv->d, "UDISKS_PARTITION_TYPE");
  if (g_strcmp0 (partition_scheme, "mbr") == 0 && (partition_type == 0x05 || partition_type == 0x0f || partition_type
                                                   == 0x85))
    {
      device_set_id_usage (device, "");
      device_set_id_type (device, "");
      device_set_id_version (device, "");
      device_set_id_label (device, "");
      device_set_id_uuid (device, "");
      goto out;
    }

  device_set_id_usage (device, g_udev_device_get_property (device->priv->d, "ID_FS_USAGE"));
  device_set_id_type (device, g_udev_device_get_property (device->priv->d, "ID_FS_TYPE"));
  device_set_id_version (device, g_udev_device_get_property (device->priv->d, "ID_FS_VERSION"));
  if (g_udev_device_has_property (device->priv->d, "ID_FS_LABEL_ENC"))
    {
      decoded_string = decode_udev_encoded_string (g_udev_device_get_property (device->priv->d, "ID_FS_LABEL_ENC"));
      device_set_id_label (device, decoded_string);
      g_free (decoded_string);
    }
  else
    {
      device_set_id_label (device, g_udev_device_get_property (device->priv->d, "ID_FS_LABEL"));
    }
  device_set_id_uuid (device, g_udev_device_get_property (device->priv->d, "ID_FS_UUID"));

 out:
  return TRUE;
}

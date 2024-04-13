partition_create_found_device (Device *device,
                               CreatePartitionData *data)
{
  if (strlen (data->fstype) > 0)
    {
      device_filesystem_create_internal (device,
                                         data->fstype,
                                         data->fsoptions,
                                         partition_create_filesystem_create_hook,
                                         NULL,
                                         data->context);
    }
  else
    {
      dbus_g_method_return (data->context, device->priv->object_path);
    }
}

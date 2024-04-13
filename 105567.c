lvm2_lv_create_found_device (Device *device,
                             CreateLvm2LVData *data)
{
  if (strlen (data->fstype) > 0)
    {
      device_filesystem_create_internal (device,
                                         data->fstype,
                                         data->fsoptions,
                                         lvm2_lv_create_filesystem_create_hook,
                                         NULL,
                                         data->context);
    }
  else
    {
      dbus_g_method_return (data->context, device->priv->object_path);
    }
}

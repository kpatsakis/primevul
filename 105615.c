update_info_drive_adapter (Device *device)
{
  Adapter *adapter;
  const gchar *adapter_object_path;

  adapter_object_path = NULL;

  adapter = daemon_local_find_enclosing_adapter (device->priv->daemon, device->priv->native_path);
  if (adapter != NULL)
    {
      adapter_object_path = adapter_local_get_object_path (adapter);
    }

  device_set_drive_adapter (device, adapter_object_path);

  return TRUE;
}

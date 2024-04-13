partition_table_create_device_changed_cb (Daemon *daemon,
                                          const char *object_path,
                                          gpointer user_data)
{
  CreatePartitionTableData *data = user_data;
  Device *device;

  device = daemon_local_find_by_object_path (daemon, object_path);
  if (device == data->device)
    {
      if (g_strcmp0 (device->priv->partition_table_scheme, data->scheme) == 0 || (device->priv->partition_table_scheme
                                                                                  == NULL && g_strcmp0 (data->scheme, "none") == 0))
        {
          dbus_g_method_return (data->context);

          g_signal_handler_disconnect (daemon, data->device_changed_signal_handler_id);
          g_source_remove (data->device_changed_timeout_id);
          partition_table_create_data_unref (data);
        }
    }
}

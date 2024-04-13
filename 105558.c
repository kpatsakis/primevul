luks_unlock_device_added_cb (Daemon *daemon,
                             const char *object_path,
                             gpointer user_data)
{
  UnlockEncryptionData *data = user_data;
  Device *device;

  /* check the device is a cleartext partition for us */
  device = daemon_local_find_by_object_path (daemon, object_path);

  if (device != NULL && device->priv->device_is_luks_cleartext && strcmp (device->priv->luks_cleartext_slave,
                                                                          data->device->priv->object_path) == 0)
    {

      g_signal_handler_disconnect (daemon, data->device_added_signal_handler_id);
      g_signal_handler_disconnect (daemon, data->device_changed_signal_handler_id);
      g_source_remove (data->device_added_timeout_id);

      /* update and emit a Changed() signal on the holder since the luks-holder
       * property indicates the cleartext device
       */
      update_info (data->device);
      drain_pending_changes (data->device, FALSE);

      if (data->hook_func != NULL)
        {
          data->hook_func (data->context, device, data->hook_user_data);
        }
      else
        {
          dbus_g_method_return (data->context, object_path);
        }

      unlock_encryption_data_unref (data);
    }
}

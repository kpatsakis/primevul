luks_unlock_start_waiting_for_cleartext_device (UnlockEncryptionData *data)
{
  Device *cleartext_device;

  cleartext_device = find_cleartext_device (data->device);
  if (cleartext_device != NULL)
    {
      /* update and emit a Changed() signal on the holder since the luks-holder
       * property indicates the cleartext device
       */
      update_info (data->device);
      drain_pending_changes (data->device, FALSE);

      if (data->hook_func != NULL)
        {
          data->hook_func (data->context, cleartext_device, data->hook_user_data);
        }
      else
        {
          dbus_g_method_return (data->context, cleartext_device->priv->object_path);
        }

      unlock_encryption_data_unref (data);
    }
  else
    {
      /* sit around wait for the cleartext device to appear */
      data->device_added_signal_handler_id = g_signal_connect_after (data->device->priv->daemon,
                                                                     "device-added",
                                                                     (GCallback) luks_unlock_device_added_cb,
                                                                     data);
      data->device_changed_signal_handler_id = g_signal_connect_after (data->device->priv->daemon,
                                                                       "device-changed",
                                                                       (GCallback) luks_unlock_device_added_cb,
                                                                       data);

      /* set up timeout for error reporting if waiting failed */
      data->device_added_timeout_id = g_timeout_add (15 * 1000, luks_unlock_device_not_seen_cb, data);

      /* Note that the signal and timeout handlers share the ref to data - one will cancel the other */
    }
}

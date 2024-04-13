luks_unlock_device_not_seen_cb (gpointer user_data)
{
  UnlockEncryptionData *data = user_data;

  g_signal_handler_disconnect (data->device->priv->daemon, data->device_added_signal_handler_id);
  g_signal_handler_disconnect (data->device->priv->daemon, data->device_changed_signal_handler_id);

  throw_error (data->context,
               ERROR_FAILED,
               "Error unlocking device: timeout (10s) waiting for cleartext device to show up");

  if (data->hook_func != NULL)
    {
      data->hook_func (data->context, NULL, data->hook_user_data);
    }

  unlock_encryption_data_unref (data);
  return FALSE;
}

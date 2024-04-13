filesystem_create_wait_for_luks_device_not_seen_cb (gpointer user_data)
{
  MkfsLuksData *data = user_data;

  throw_error (data->context,
               ERROR_FAILED,
               "Error creating luks encrypted file system: timeout (10s) waiting for luks device to show up");

  g_signal_handler_disconnect (data->device->priv->daemon, data->device_changed_signal_handler_id);
  mkfse_data_unref (data);

  return FALSE;
}

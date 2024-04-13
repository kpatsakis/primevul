luks_lock_wait_for_cleartext_device_not_seen_cb (gpointer user_data)
{
  LockEncryptionData *data = user_data;

  job_local_end (data->luks_device);

  throw_error (data->context,
               ERROR_FAILED,
               "Error locking luks device: timeout (10s) waiting for cleartext device to be removed");

  g_signal_handler_disconnect (data->cleartext_device->priv->daemon, data->device_removed_signal_handler_id);
  lock_encryption_data_unref (data);
  return FALSE;
}

filesystem_create_wait_for_luks_device_changed_cb (Daemon *daemon,
                                                   const char *object_path,
                                                   gpointer user_data)
{
  MkfsLuksData *data = user_data;
  Device *device;

  /* check if we're now a LUKS crypto device */
  device = daemon_local_find_by_object_path (daemon, object_path);
  if (device == data->device && (device->priv->id_usage != NULL && strcmp (device->priv->id_usage, "crypto") == 0)
      && (device->priv->id_type != NULL && strcmp (device->priv->id_type, "crypto_LUKS") == 0))
    {

      /* yay! we are now set up the corresponding cleartext device */

      device_luks_unlock_internal (data->device,
                                   data->passphrase,
                                   NULL,
                                   filesystem_create_wait_for_cleartext_device_hook,
                                   data,
                                   data->context);

      g_signal_handler_disconnect (daemon, data->device_changed_signal_handler_id);
      g_source_remove (data->device_changed_timeout_id);
    }
}

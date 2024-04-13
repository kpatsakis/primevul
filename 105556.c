luks_lock_wait_for_cleartext_device_removed_cb (Daemon *daemon,
                                                const char *object_path,
                                                gpointer user_data)
{
  Device *device;
  LockEncryptionData *data = user_data;

  device = daemon_local_find_by_object_path (daemon, object_path);
  if (device == data->cleartext_device)
    {

      job_local_end (data->luks_device);

      /* update and emit a Changed() signal on the holder since the luks-holder
       * property indicates the cleartext device
       */
      update_info (data->luks_device);
      drain_pending_changes (data->luks_device, FALSE);

      dbus_g_method_return (data->context);

      g_signal_handler_disconnect (daemon, data->device_removed_signal_handler_id);
      g_source_remove (data->device_removed_timeout_id);
      lock_encryption_data_unref (data);
    }
}

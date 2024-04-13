linux_md_start_device_added_cb (Daemon *daemon,
                                const char *object_path,
                                gpointer user_data)
{
  LinuxMdStartData *data = user_data;
  Device *device;

  /* check the device is the one we're looking for */
  device = daemon_local_find_by_object_path (daemon, object_path);

  if (device != NULL && device->priv->device_is_linux_md)
    {

      /* TODO: actually check this properly by looking at slaves vs. components */

      /* yay! it is.. return value to the user */
      dbus_g_method_return (data->context, object_path);

      g_signal_handler_disconnect (daemon, data->device_added_signal_handler_id);
      g_source_remove (data->device_added_timeout_id);
      linux_md_start_data_unref (data);
    }
}

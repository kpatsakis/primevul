linux_md_remove_component_device_changed_cb (Daemon *daemon,
                                             const char *object_path,
                                             gpointer user_data)
{
  RemoveComponentData *data = user_data;
  Device *device;
  GError *error;

  error = NULL;

  device = daemon_local_find_by_object_path (daemon, object_path);
  if (device == data->slave)
    {

      if (device_local_is_busy (data->slave, FALSE, &error))
        {
          dbus_g_method_return_error (data->context, error);
          g_error_free (error);
        }
      else
        {
          gchar *fs_create_options[] =
            { NULL };

          /* yay! now scrub it! */
          device_filesystem_create (data->slave, "empty", fs_create_options, data->context);

          /* TODO: leaking data? */

          g_signal_handler_disconnect (daemon, data->device_changed_signal_handler_id);
          g_source_remove (data->device_changed_timeout_id);
        }
    }
}

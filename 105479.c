filesystem_create_completed_cb (DBusGMethodInvocation *context,
                                Device *device,
                                gboolean job_was_cancelled,
                                int status,
                                const char *stderr,
                                const char *stdout,
                                gpointer user_data)
{
  MkfsData *data = user_data;

  /* poke the kernel so we can reread the data */
  device_generate_kernel_change_event (device);

  if (WEXITSTATUS (status) == 0 && !job_was_cancelled)
    {
      if (data->hook_func != NULL)
        data->hook_func (context, device, TRUE, data->hook_user_data);
      else
        dbus_g_method_return (context);
    }
  else
    {
      if (job_was_cancelled)
        {
          throw_error (context, ERROR_CANCELLED, "Job was cancelled");
        }
      else if (WEXITSTATUS (status) == 3)
        {
          throw_error (context,
                       ERROR_FILESYSTEM_TOOLS_MISSING,
                       "Error creating file system: Cannot run mkfs: %s",
                       stderr);
        }
      else
        {
          throw_error (context,
                       ERROR_FAILED,
                       "Error creating file system: helper exited with exit code %d: %s\n%s",
                       WEXITSTATUS (status),
                       stderr,
                       stdout);
        }

      if (data->hook_func != NULL)
        data->hook_func (context, device, FALSE, data->hook_user_data);
    }
}

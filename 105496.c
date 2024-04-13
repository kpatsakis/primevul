force_luks_teardown_completed_cb (DBusGMethodInvocation *context,
                                  Device *device,
                                  gboolean job_was_cancelled,
                                  int status,
                                  const char *stderr,
                                  const char *stdout,
                                  gpointer user_data)
{
  ForceLuksTeardownData *data = user_data;

  if (WEXITSTATUS (status) == 0 && !job_was_cancelled)
    {

      g_print ("**** NOTE: Successfully teared down luks device %s\n", device->priv->device_file);

      if (data->fr_callback != NULL)
        data->fr_callback (device, TRUE, data->fr_user_data);
    }
  else
    {
      g_print ("**** NOTE: force luks teardown failed: %s\n", stderr);
      if (data->fr_callback != NULL)
        data->fr_callback (device, FALSE, data->fr_user_data);
    }
}

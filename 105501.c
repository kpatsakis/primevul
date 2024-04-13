force_unmount_completed_cb (DBusGMethodInvocation *context,
                            Device *device,
                            gboolean job_was_cancelled,
                            int status,
                            const char *stderr,
                            const char *stdout,
                            gpointer user_data)
{
  ForceUnmountData *data = user_data;

  if (WEXITSTATUS (status) == 0 && !job_was_cancelled)
    {

      g_print ("**** NOTE: Successfully force unmounted device %s\n", device->priv->device_file);
      /* update_info_mount_state() will update the mounts file and clean up the directory if needed */
      update_info (device);

      if (data->fr_callback != NULL)
        data->fr_callback (device, TRUE, data->fr_user_data);
    }
  else
    {
      g_print ("**** NOTE: force unmount failed: %s\n", stderr);
      if (data->fr_callback != NULL)
        data->fr_callback (device, FALSE, data->fr_user_data);
    }
}

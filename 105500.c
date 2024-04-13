force_unmount (Device *device,
               ForceRemovalCompleteFunc callback,
               gpointer user_data)
{
  int n;
  char *argv[16];
  const gchar *mount_path;

  mount_path = ((gchar **) device->priv->device_mount_paths->pdata)[0];

  n = 0;
  argv[n++] = "umount";
  /* on Linux, we only have lazy unmount for now */
  argv[n++] = "-l";
  argv[n++] = (gchar *) mount_path;
  argv[n++] = NULL;

  if (!job_new (NULL,
                "ForceUnmount",
                FALSE,
                device,
                argv,
                NULL,
                force_unmount_completed_cb,
                FALSE,
                force_unmount_data_new (mount_path, callback, user_data),
                (GDestroyNotify) force_unmount_data_unref))
    {
      g_warning ("Couldn't spawn unmount for force unmounting %s", mount_path);
      if (callback != NULL)
        callback (device, FALSE, user_data);
    }
}

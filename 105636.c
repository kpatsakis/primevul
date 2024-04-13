update_info_mount_state (Device *device)
{
  MountMonitor *monitor;
  GList *mounts;
  gboolean was_mounted;

  mounts = NULL;

  /* defer setting the mount point until FilesystemMount returns and
   * the mounts file is written
   */
  if (device->priv->job_in_progress && g_strcmp0 (device->priv->job_id, "FilesystemMount") == 0)
    goto out;

  monitor = daemon_local_get_mount_monitor (device->priv->daemon);

  mounts = mount_monitor_get_mounts_for_dev (monitor, device->priv->dev);

  was_mounted = device->priv->device_is_mounted;

  if (mounts != NULL)
    {
      GList *l;
      guint n;
      gchar **mount_paths;

      mount_paths = g_new0 (gchar *, g_list_length (mounts) + 1);
      for (l = mounts, n = 0; l != NULL; l = l->next, n++)
        {
          mount_paths[n] = g_strdup (mount_get_mount_path (MOUNT (l->data)));
        }

      device_set_device_is_mounted (device, TRUE);
      device_set_device_mount_paths (device, mount_paths);
      if (!was_mounted)
        {
          uid_t mounted_by_uid;

          if (!mount_file_has_device (device->priv->device_file, &mounted_by_uid, NULL))
            mounted_by_uid = 0;
          device_set_device_mounted_by_uid (device, mounted_by_uid);
        }

      g_strfreev (mount_paths);

    }
  else
    {
      gboolean remove_dir_on_unmount;
      gchar *old_mount_path;

      old_mount_path = NULL;
      if (device->priv->device_mount_paths->len > 0)
        old_mount_path = g_strdup (((gchar **) device->priv->device_mount_paths->pdata)[0]);

      device_set_device_is_mounted (device, FALSE);
      device_set_device_mount_paths (device, NULL);
      device_set_device_mounted_by_uid (device, 0);

      /* clean up stale mount directory */
      remove_dir_on_unmount = FALSE;
      if (was_mounted && mount_file_has_device (device->priv->device_file, NULL, &remove_dir_on_unmount))
        {
          mount_file_remove (device->priv->device_file, old_mount_path);
          if (remove_dir_on_unmount)
            {
              if (g_rmdir (old_mount_path) != 0)
                {
                  g_warning ("Error removing dir '%s' on unmount: %m", old_mount_path);
                }
            }
        }

      g_free (old_mount_path);

    }

 out:
  g_list_foreach (mounts, (GFunc) g_object_unref, NULL);
  g_list_free (mounts);

  return TRUE;
}

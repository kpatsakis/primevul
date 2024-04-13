is_device_in_fstab (Device *device,
                    char **out_mount_point)
{
  GList *l;
  GList *mount_points;
  gboolean ret;

  ret = FALSE;

  mount_points = g_unix_mount_points_get (NULL);
  for (l = mount_points; l != NULL; l = l->next)
    {
      GUnixMountPoint *mount_point = l->data;
      char canonical_device_file[PATH_MAX];
      char *device_path;
      char *s;

      device_path = g_strdup (g_unix_mount_point_get_device_path (mount_point));

      /* get the canonical path; e.g. resolve
       *
       * /dev/disk/by-path/pci-0000:00:1d.7-usb-0:3:1.0-scsi-0:0:0:3-part5
       * UUID=78af6939-adac-4ea5-a2a8-576e141da010
       * LABEL=foobar
       *
       * into something like /dev/sde5.
       */
      if (g_str_has_prefix (device_path, "UUID="))
        {
          s = device_path;
          device_path = g_strdup_printf ("/dev/disk/by-uuid/%s", device_path + 5);
          g_free (s);
        }
      else if (g_str_has_prefix (device_path, "LABEL="))
        {
          s = device_path;
          device_path = g_strdup_printf ("/dev/disk/by-label/%s", device_path + 6);
          g_free (s);
        }

      if (realpath (device_path, canonical_device_file) == NULL)
        {
          g_free (device_path);
          continue;
        }
      g_free (device_path);

      if (strcmp (device->priv->device_file, canonical_device_file) == 0)
        {
          ret = TRUE;
          if (out_mount_point != NULL)
            *out_mount_point = g_strdup (g_unix_mount_point_get_mount_path (mount_point));
          break;
        }
    }
  g_list_foreach (mount_points, (GFunc) g_unix_mount_point_free, NULL);
  g_list_free (mount_points);

  return ret;
}

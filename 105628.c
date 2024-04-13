update_info_linux_loop (Device *device)
{
  gboolean ret;
  gint fd;
  struct loop_info64 loop_info_buf;
  gboolean is_loop;
  gchar *s;
  gchar *s2;

  is_loop = FALSE;
  ret = FALSE;
  fd = -1;

  if (!g_str_has_prefix (device->priv->native_path, "/sys/devices/virtual/block/loop"))
    {
      ret = TRUE;
      goto out;
    }

  fd = open (device->priv->device_file, O_RDONLY);
  if (fd < 0)
    goto out;

  if (ioctl (fd, LOOP_GET_STATUS64, &loop_info_buf) != 0)
    goto out;

  /* TODO: is lo_file_name really NUL-terminated? */
  device_set_linux_loop_filename (device, (const gchar *) loop_info_buf.lo_file_name);

  ret = TRUE;
  is_loop = TRUE;

  device_set_drive_vendor (device, "Linux");
  s2 = g_path_get_basename ((gchar *) loop_info_buf.lo_file_name);
  s = g_strdup_printf ("Loop: %s", s2);
  g_free (s2);
  device_set_drive_model (device, s);
  g_free (s);

 out:
  if (fd > 0)
    close (fd);
  device_set_device_is_linux_loop (device, is_loop);
  return ret;
}

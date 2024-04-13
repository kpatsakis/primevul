device_linux_md_expand (Device *device,
                        GPtrArray *components,
                        char **options,
                        DBusGMethodInvocation *context)
{
  gchar **strv;
  guint n;

  if (!device->priv->device_is_linux_md)
    {
      throw_error (context, ERROR_FAILED, "Device is not a Linux md drive");
      goto out;
    }

  strv = (gchar **) g_new0 (gchar*, components->len + 1);
  for (n = 0; n < components->len; n++)
    strv[n] = g_strdup (components->pdata[n]);

  daemon_local_check_auth (device->priv->daemon,
                           device,
                           "org.freedesktop.udisks.linux-md",
                           "LinuxMdExpand",
                           TRUE,
                           device_linux_md_expand_authorized_cb,
                           context,
                           2,
                           strv,
                           g_strfreev,
                           g_strdupv (options),
                           g_strfreev);
 out:
  return TRUE;
}

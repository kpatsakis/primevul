device_linux_md_check (Device *device,
                       char **options,
                       DBusGMethodInvocation *context)
{
  guint n;
  const gchar *job_name;

  job_name = "LinuxMdCheck";
  for (n = 0; options != NULL && options[n] != NULL; n++)
    if (strcmp (options[n], "repair") == 0)
      job_name = "LinuxMdRepair";

  if (!device->priv->device_is_linux_md)
    {
      throw_error (context, ERROR_FAILED, "Device is not a Linux md drive");
      goto out;
    }

  if (g_strcmp0 (device->priv->linux_md_sync_action, "idle") != 0)
    {
      throw_error (context, ERROR_FAILED, "Array is not idle");
      goto out;
    }

  daemon_local_check_auth (device->priv->daemon,
                           device,
                           "org.freedesktop.udisks.linux-md",
                           job_name,
                           TRUE,
                           device_linux_md_check_authorized_cb,
                           context,
                           1,
                           g_strdupv (options),
                           g_strfreev);

 out:
  return TRUE;
}

device_linux_md_stop (Device *device,
                      char **options,
                      DBusGMethodInvocation *context)
{
  if (!device->priv->device_is_linux_md)
    {
      throw_error (context, ERROR_FAILED, "Device is not a Linux md drive");
      goto out;
    }

  daemon_local_check_auth (device->priv->daemon,
                           device,
                           "org.freedesktop.udisks.linux-md",
                           "LinuxMdStop",
                           TRUE,
                           device_linux_md_stop_authorized_cb,
                           context,
                           1,
                           g_strdupv (options),
                           g_strfreev);

 out:
  return TRUE;
}

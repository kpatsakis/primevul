device_linux_md_remove_component (Device *device,
                                  char *component,
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
                           "LinuxMdRemoveComponent",
                           TRUE,
                           device_linux_md_remove_component_authorized_cb,
                           context,
                           2,
                           g_strdup (component),
                           g_free,
                           g_strdupv (options),
                           g_strfreev);
 out:
  return TRUE;
}

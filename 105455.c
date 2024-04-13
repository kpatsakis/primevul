device_luks_unlock (Device *device,
                    const char *secret,
                    char **options,
                    DBusGMethodInvocation *context)
{
  if (device->priv->id_usage == NULL || strcmp (device->priv->id_usage, "crypto") != 0)
    {
      throw_error (context, ERROR_FAILED, "Not a LUKS device");
      goto out;
    }

  daemon_local_check_auth (device->priv->daemon,
                           device,
                           "org.freedesktop.udisks.luks-unlock",
                           "LuksUnlock",
                           TRUE,
                           device_luks_unlock_authorized_cb,
                           context,
                           2,
                           g_strdup (secret),
                           g_free,
                           g_strdupv (options),
                           g_strfreev);

 out:
  return TRUE;
}

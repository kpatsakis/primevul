device_luks_change_passphrase (Device *device,
                               const char *old_secret,
                               const char *new_secret,
                               DBusGMethodInvocation *context)
{
  /* No need to check for busy; we can actually do this while the device is unlocked as
   * only LUKS metadata is modified.
   */

  if (device->priv->id_usage == NULL || strcmp (device->priv->id_usage, "crypto") != 0)
    {
      throw_error (context, ERROR_FAILED, "Not a LUKS crypto device");
      goto out;
    }

  daemon_local_check_auth (device->priv->daemon,
                           device,
                           device->priv->device_is_system_internal ? "org.freedesktop.udisks.change-system-internal"
                           : "org.freedesktop.udisks.change",
                           "LuksChangePassphrase",
                           TRUE,
                           device_luks_change_passphrase_authorized_cb,
                           context,
                           2,
                           g_strdup (old_secret),
                           g_free,
                           g_strdup (new_secret),
                           g_free);

 out:
  return TRUE;
}

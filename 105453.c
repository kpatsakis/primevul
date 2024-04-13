device_luks_lock (Device *device,
                  char **options,
                  DBusGMethodInvocation *context)
{
  uid_t unlocked_by_uid;
  uid_t uid;
  Device *cleartext_device;
  const gchar *action_id;

  daemon_local_get_uid (device->priv->daemon, &uid, context);

  if (device->priv->id_usage == NULL || strcmp (device->priv->id_usage, "crypto") != 0)
    {
      throw_error (context, ERROR_FAILED, "Not a LUKS crypto device");
      goto out;
    }

  cleartext_device = find_cleartext_device (device);
  if (cleartext_device == NULL)
    {
      throw_error (context, ERROR_FAILED, "Cleartext device is not unlocked");
      goto out;
    }

  if (cleartext_device->priv->dm_name == NULL || strlen (cleartext_device->priv->dm_name) == 0)
    {
      throw_error (context, ERROR_FAILED, "Cannot determine device-mapper name");
      goto out;
    }

  /* see if we (e.g. udisks) set up this clear text device */
  if (!luks_get_uid_from_dm_name (cleartext_device->priv->dm_name, &unlocked_by_uid))
    {
      /* nope.. so assume uid 0 set it up.. we still allow locking
       * the device... given enough privilege
       */
      unlocked_by_uid = 0;
    }

  /* require authorization if unlocked by someone else */
  action_id = NULL;
  if (unlocked_by_uid != uid)
    {
      action_id = "org.freedesktop.udisks.luks-lock-others";
    }

  daemon_local_check_auth (device->priv->daemon,
                           device,
                           action_id,
                           "LuksLock",
                           TRUE,
                           device_luks_lock_authorized_cb,
                           context,
                           1,
                           g_strdupv (options),
                           g_strfreev);

 out:
  return TRUE;
}

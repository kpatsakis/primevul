force_removal (Device *device,
               ForceRemovalCompleteFunc callback,
               gpointer user_data)
{

  /* Device is going bye bye. If this device is
   *
   *  - Mounted by us, then forcibly unmount it.
   *
   *  - If it's a luks device, check if there's cleartext
   *    companion. If so, tear it down if it was setup by us.
   *
   */
  if (device->priv->device_is_mounted && device->priv->device_mount_paths->len > 0)
    {
      gboolean remove_dir_on_unmount;

      if (mount_file_has_device (device->priv->device_file, NULL, &remove_dir_on_unmount) ||
          is_device_in_fstab (device, NULL))
        {
          g_print ("**** NOTE: Force unmounting device %s\n", device->priv->device_file);
          force_unmount (device, callback, user_data);
          goto pending;
        }
    }

  if (device->priv->id_usage != NULL && strcmp (device->priv->id_usage, "crypto") == 0)
    {
      GList *devices;
      GList *l;

      /* look for cleartext device  */
      devices = daemon_local_get_all_devices (device->priv->daemon);
      for (l = devices; l != NULL; l = l->next)
        {
          Device *d = DEVICE (l->data);
          if (d->priv->device_is_luks_cleartext && d->priv->luks_cleartext_slave != NULL
              && strcmp (d->priv->luks_cleartext_slave, device->priv->object_path) == 0)
            {

              /* Check whether it is set up by us */
              if (d->priv->dm_name != NULL && g_str_has_prefix (d->priv->dm_name, "udisks-luks-uuid-"))
                {

                  g_print ("**** NOTE: Force luks teardown device %s (cleartext %s)\n",
                           device->priv->device_file,
                           d->priv->device_file);

                  /* Gotcha */
                  force_luks_teardown (device, d, callback, user_data);
                  goto pending;
                }
            }
        }
    }

  /* nothing to force remove */
  if (callback != NULL)
    callback (device, TRUE, user_data);

 pending:
  ;
}

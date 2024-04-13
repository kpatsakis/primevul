device_changed (Device *device,
                GUdevDevice *d,
                gboolean synthesized)
{
  gboolean keep_device;

  g_object_unref (device->priv->d);
  device->priv->d = g_object_ref (d);

  keep_device = update_info (device);

  /* this 'change' event might prompt us to remove the device */
  if (!keep_device)
    goto out;

  /* no, it's good .. keep it.. and always force a 'change' signal if the event isn't synthesized */
  drain_pending_changes (device, !synthesized);

  /* Check if media was removed. If so, we possibly need to clean up
   * if the device itself is busy. This includes
   *
   *  - force unmounting the device
   *
   *  - tearing down a luks mapping if it's a cleartext device
   *    backed by a crypted device
   *
   * but see force_removal() for details.
   *
   * This is the normally the path where the media is removed but the enclosing
   * device is still present. Compare with device_removed() for
   * the other path.
   */
  if (!device->priv->device_is_media_available)
    {
      GList *l;
      GList *devices;

      force_removal (device, NULL, NULL);

      /* check all partitions */
      devices = daemon_local_get_all_devices (device->priv->daemon);
      for (l = devices; l != NULL; l = l->next)
        {
          Device *d = DEVICE (l->data);

          if (d->priv->device_is_partition && d->priv->partition_slave != NULL && strcmp (d->priv->partition_slave,
                                                                                          device->priv->object_path)
              == 0)
            {

              force_removal (d, NULL, NULL);
            }
        }

      g_list_free (devices);
    }
 out:
  return keep_device;
}

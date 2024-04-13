find_cleartext_device (Device *device)
{
  GList *devices;
  GList *l;
  Device *ret;

  ret = NULL;

  /* check that there isn't a cleartext device already  */
  devices = daemon_local_get_all_devices (device->priv->daemon);
  for (l = devices; l != NULL; l = l->next)
    {
      Device *d = DEVICE (l->data);
      if (d->priv->device_is_luks_cleartext && d->priv->luks_cleartext_slave != NULL
          && strcmp (d->priv->luks_cleartext_slave, device->priv->object_path) == 0)
        {
          ret = d;
          goto out;
        }
    }

 out:

  g_list_free (devices);

  return ret;
}

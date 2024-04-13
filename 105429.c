device_has_logical_partitions (Device *device)
{
  gboolean ret;
  GList *l;
  GList *devices;

  ret = FALSE;

  devices = daemon_local_get_all_devices (device->priv->daemon);
  for (l = devices; l != NULL; l = l->next)
    {
      Device *d = DEVICE (l->data);

      if (d->priv->device_is_partition && d->priv->partition_slave != NULL && g_strcmp0 (d->priv->partition_slave,
                                                                                         device->priv->object_path)
          == 0 && g_strcmp0 (d->priv->partition_scheme, "mbr") == 0 && d->priv->partition_number >= 5)
        {

          ret = TRUE;
        }
    }

  g_list_free (devices);

  return ret;
}

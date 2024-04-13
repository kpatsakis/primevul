update_info_drive_similar_devices (Device *device)
{
  GList *devices;
  GList *l;
  GPtrArray *p;

  p = g_ptr_array_new ();

  if (!device->priv->device_is_drive)
    goto out;

  /* We need non-empty SERIAL and WWN for this to work */
  if ((device->priv->drive_serial == NULL || strlen (device->priv->drive_serial) == 0) ||
      (device->priv->drive_wwn == NULL || strlen (device->priv->drive_wwn) == 0))
    goto out;

  /* TODO: this might be slow - if so, use a hash on the Daemon class */
  devices = daemon_local_get_all_devices (device->priv->daemon);
  for (l = devices; l != NULL; l = l->next)
    {
      Device *d = DEVICE (l->data);

      if (!d->priv->device_is_drive)
        continue;

      if (d == device)
        continue;

#if 0
      g_debug ("looking at %s:\n"
               "  %s\n"
               "  %s\n"
               "    %s\n"
               "    %s\n",
               d->priv->device_file,
               d->priv->drive_serial,
               device->priv->drive_serial,
               d->priv->drive_wwn,
               device->priv->drive_wwn);
#endif

      /* current policy is that *both* SERIAL and WWN must match */
      if (g_strcmp0 (d->priv->drive_serial, device->priv->drive_serial) == 0 &&
          g_strcmp0 (d->priv->drive_wwn, device->priv->drive_wwn) == 0)
        {
          g_ptr_array_add (p, d->priv->object_path);

          /* ensure that the device we added also exists in its own drive_similar_devices property */
          if (!ptr_array_has_string (d->priv->drive_similar_devices, device->priv->object_path))
            {
              update_info_in_idle (d);
            }
          else
            {
            }
        }
    }

 out:
  g_ptr_array_add (p, NULL);
  device_set_drive_similar_devices (device, (GStrv) p->pdata);
  g_ptr_array_free (p, TRUE);
  return TRUE;
}

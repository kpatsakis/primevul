update_info_linux_dmmp (Device *device)
{
  const gchar *dm_name;
  const gchar* const *target_types;
  const gchar* const *target_parameters;
  gchar *decoded_params;
  gboolean is_dmmp;
  guint n;
  GPtrArray *p;
  Device *component;
  gchar *s;

  is_dmmp = FALSE;
  p = NULL;
  decoded_params = NULL;

  dm_name = g_udev_device_get_property (device->priv->d, "DM_NAME");
  if (dm_name == NULL)
    goto out;

  target_types = g_udev_device_get_property_as_strv (device->priv->d, "UDISKS_DM_TARGETS_TYPE");
  if (target_types == NULL || g_strcmp0 (target_types[0], "multipath") != 0)
    goto out;

  if (device->priv->slaves_objpath->len == 0)
    goto out;

  target_parameters = g_udev_device_get_property_as_strv (device->priv->d, "UDISKS_DM_TARGETS_PARAMS");
  if (target_parameters == NULL || g_strv_length ((gchar **) target_parameters) != 1)
    goto out;
  decoded_params = decode_udev_encoded_string (target_parameters[0]);

  device_set_linux_dmmp_name (device, dm_name);

  device_set_linux_dmmp_parameters (device, decoded_params);

  p = g_ptr_array_new ();
  component = NULL;
  for (n = 0; n < device->priv->slaves_objpath->len; n++)
    {
      const gchar *component_objpath = device->priv->slaves_objpath->pdata[n];
      if (component == NULL)
        {
          component = daemon_local_find_by_object_path (device->priv->daemon, component_objpath);
        }
      g_ptr_array_add (p, (gpointer) component_objpath);
    }
  g_ptr_array_add (p, NULL);
  device_set_linux_dmmp_slaves (device, (GStrv) p->pdata);

  if (component == NULL)
    goto out;

  /* Copy only drive properties used for identification to the multipath device. Yes,
   * this means, we'll get serial/wwn clashes but this is already so for each path.
   *
   * Also, clients *should* be smart about things and special-handle linux_dmmp and
   * linux_dmmp_component devices.
   */
  device_set_drive_vendor (device, component->priv->drive_vendor);
  device_set_drive_model (device, component->priv->drive_model);
  device_set_drive_revision (device, component->priv->drive_revision);
  device_set_drive_serial (device, component->priv->drive_serial);
  device_set_drive_wwn (device, component->priv->drive_wwn);

  /* connection interface */
  device_set_drive_connection_interface (device, "virtual_multipath");
  device_set_drive_connection_speed (device, 0);

  s = g_strdup_printf ("/dev/mapper/%s", dm_name);
  device_set_device_file_presentation (device, s);
  g_free (s);

  is_dmmp = TRUE;

 out:
  g_free (decoded_params);
  if (p != NULL)
    g_ptr_array_free (p, TRUE);
  device_set_device_is_linux_dmmp (device, is_dmmp);
  return TRUE;
}

device_finalize (GObject *object)
{
  Device *device;
  GList *l;

  g_return_if_fail (object != NULL);
  g_return_if_fail (IS_DEVICE (object));

  device = DEVICE (object);
  g_return_if_fail (device->priv != NULL);

  /* g_debug ("finalizing %s", device->priv->native_path); */

  g_object_unref (device->priv->d);
  g_object_unref (device->priv->daemon);
  g_free (device->priv->object_path);

  g_free (device->priv->native_path);

  for (l = device->priv->polling_inhibitors; l != NULL; l = l->next)
    {
      Inhibitor *inhibitor = INHIBITOR (l->data);
      g_signal_handlers_disconnect_by_func (inhibitor, polling_inhibitor_disconnected_cb, device);
      g_object_unref (inhibitor);
    }
  g_list_free (device->priv->polling_inhibitors);

  for (l = device->priv->spindown_inhibitors; l != NULL; l = l->next)
    {
      Inhibitor *inhibitor = INHIBITOR (l->data);
      g_signal_handlers_disconnect_by_func (inhibitor, spindown_inhibitor_disconnected_cb, device);
      g_object_unref (inhibitor);
    }
  g_list_free (device->priv->spindown_inhibitors);

  if (device->priv->linux_md_poll_timeout_id > 0)
    g_source_remove (device->priv->linux_md_poll_timeout_id);

  if (device->priv->emit_changed_idle_id > 0)
    g_source_remove (device->priv->emit_changed_idle_id);

  /* free properties */
  g_free (device->priv->device_file);
  g_free (device->priv->device_file_presentation);
  g_ptr_array_foreach (device->priv->device_file_by_id, (GFunc) g_free, NULL);
  g_ptr_array_foreach (device->priv->device_file_by_path, (GFunc) g_free, NULL);
  g_ptr_array_free (device->priv->device_file_by_id, TRUE);
  g_ptr_array_free (device->priv->device_file_by_path, TRUE);
  g_ptr_array_free (device->priv->device_mount_paths, TRUE);
  g_free (device->priv->device_presentation_name);
  g_free (device->priv->device_presentation_icon_name);

  g_free (device->priv->id_usage);
  g_free (device->priv->id_type);
  g_free (device->priv->id_version);
  g_free (device->priv->id_uuid);
  g_free (device->priv->id_label);

  g_free (device->priv->partition_slave);
  g_free (device->priv->partition_scheme);
  g_free (device->priv->partition_type);
  g_free (device->priv->partition_label);
  g_free (device->priv->partition_uuid);
  g_ptr_array_foreach (device->priv->partition_flags, (GFunc) g_free, NULL);
  g_ptr_array_free (device->priv->partition_flags, TRUE);

  g_free (device->priv->partition_table_scheme);

  g_free (device->priv->luks_holder);

  g_free (device->priv->luks_cleartext_slave);

  g_free (device->priv->drive_vendor);
  g_free (device->priv->drive_model);
  g_free (device->priv->drive_revision);
  g_free (device->priv->drive_serial);
  g_free (device->priv->drive_wwn);
  g_free (device->priv->drive_connection_interface);
  g_ptr_array_foreach (device->priv->drive_media_compatibility, (GFunc) g_free, NULL);
  g_ptr_array_free (device->priv->drive_media_compatibility, TRUE);
  g_free (device->priv->drive_media);
  g_free (device->priv->drive_write_cache);
  g_free (device->priv->drive_adapter);
  g_ptr_array_foreach (device->priv->drive_ports, (GFunc) g_free, NULL);
  g_ptr_array_free (device->priv->drive_ports, TRUE);
  g_ptr_array_foreach (device->priv->drive_similar_devices, (GFunc) g_free, NULL);
  g_ptr_array_free (device->priv->drive_similar_devices, TRUE);

  g_free (device->priv->linux_md_component_level);
  g_free (device->priv->linux_md_component_uuid);
  g_free (device->priv->linux_md_component_home_host);
  g_free (device->priv->linux_md_component_name);
  g_free (device->priv->linux_md_component_version);
  g_free (device->priv->linux_md_component_holder);
  g_ptr_array_foreach (device->priv->linux_md_component_state, (GFunc) g_free, NULL);
  g_ptr_array_free (device->priv->linux_md_component_state, TRUE);

  g_free (device->priv->linux_md_state);
  g_free (device->priv->linux_md_level);
  g_free (device->priv->linux_md_uuid);
  g_free (device->priv->linux_md_home_host);
  g_free (device->priv->linux_md_name);
  g_free (device->priv->linux_md_version);
  g_ptr_array_foreach (device->priv->linux_md_slaves, (GFunc) g_free, NULL);
  g_ptr_array_free (device->priv->linux_md_slaves, TRUE);

  g_free (device->priv->linux_dmmp_component_holder);

  g_free (device->priv->linux_dmmp_name);
  g_ptr_array_foreach (device->priv->linux_dmmp_slaves, (GFunc) g_free, NULL);
  g_ptr_array_free (device->priv->linux_dmmp_slaves, TRUE);
  g_free (device->priv->linux_dmmp_parameters);

  g_free (device->priv->linux_loop_filename);

  g_free (device->priv->linux_lvm2_lv_name);
  g_free (device->priv->linux_lvm2_lv_uuid);
  g_free (device->priv->linux_lvm2_lv_group_name);
  g_free (device->priv->linux_lvm2_lv_group_uuid);

  g_free (device->priv->linux_lvm2_pv_uuid);
  g_free (device->priv->linux_lvm2_pv_group_name);
  g_free (device->priv->linux_lvm2_pv_group_uuid);
  g_ptr_array_foreach (device->priv->linux_lvm2_pv_group_physical_volumes, (GFunc) g_free, NULL);
  g_ptr_array_free (device->priv->linux_lvm2_pv_group_physical_volumes, TRUE);
  g_ptr_array_foreach (device->priv->linux_lvm2_pv_group_logical_volumes, (GFunc) g_free, NULL);
  g_ptr_array_free (device->priv->linux_lvm2_pv_group_logical_volumes, TRUE);

  g_free (device->priv->drive_ata_smart_blob);

  g_free (device->priv->dm_name);
  g_ptr_array_foreach (device->priv->slaves_objpath, (GFunc) g_free, NULL);
  g_ptr_array_free (device->priv->slaves_objpath, TRUE);
  g_ptr_array_foreach (device->priv->holders_objpath, (GFunc) g_free, NULL);
  g_ptr_array_free (device->priv->holders_objpath, TRUE);

  G_OBJECT_CLASS (device_parent_class)->finalize (object);
}

update_info (Device *device)
{
  char *s;
  guint n;
  gboolean ret;
  char *path;
  GDir *dir;
  const char *name;
  GList *l;
  GList *old_slaves_objpath;
  GList *old_holders_objpath;
  GList *cur_slaves_objpath;
  GList *cur_holders_objpath;
  GList *added_objpath;
  GList *removed_objpath;
  GPtrArray *symlinks_by_id;
  GPtrArray *symlinks_by_path;
  GPtrArray *slaves;
  GPtrArray *holders;
  gint major;
  gint minor;
  gboolean media_available;

  ret = FALSE;

  PROFILE ("update_info(device=%s) start", device->priv->native_path);

  g_print ("**** UPDATING %s\n", device->priv->native_path);

  /* need the slaves/holders to synthesize 'change' events if a device goes away (since the kernel
   * doesn't do generate these)
   */
  old_slaves_objpath = dup_list_from_ptrarray (device->priv->slaves_objpath);
  old_holders_objpath = dup_list_from_ptrarray (device->priv->holders_objpath);

  /* drive identification */
  if (sysfs_file_exists (device->priv->native_path, "range"))
    {
      device_set_device_is_drive (device, TRUE);
    }
  else
    {
      device_set_device_is_drive (device, FALSE);
    }

  if (!g_udev_device_has_property (device->priv->d, "MAJOR") || !g_udev_device_has_property (device->priv->d, "MINOR"))
    {
      g_warning ("No major/minor for %s", device->priv->native_path);
      goto out;
    }

  /* ignore dm devices that are suspended */
  if (g_str_has_prefix (g_udev_device_get_name (device->priv->d), "dm-"))
    {
      if (g_strcmp0 (g_udev_device_get_property (device->priv->d, "DM_SUSPENDED"), "1") == 0)
	goto out;
    }

  major = g_udev_device_get_property_as_int (device->priv->d, "MAJOR");
  minor = g_udev_device_get_property_as_int (device->priv->d, "MINOR");
  device->priv->dev = makedev (major, minor);

  device_set_device_file (device, g_udev_device_get_device_file (device->priv->d));
  if (device->priv->device_file == NULL)
    {
      g_warning ("No device file for %s", device->priv->native_path);
      goto out;
    }

  const char * const * symlinks;
  symlinks = g_udev_device_get_device_file_symlinks (device->priv->d);
  symlinks_by_id = g_ptr_array_new ();
  symlinks_by_path = g_ptr_array_new ();
  for (n = 0; symlinks[n] != NULL; n++)
    {
      if (g_str_has_prefix (symlinks[n], "/dev/disk/by-id/") || g_str_has_prefix (symlinks[n], "/dev/disk/by-uuid/"))
        {
          g_ptr_array_add (symlinks_by_id, (gpointer) symlinks[n]);
        }
      else if (g_str_has_prefix (symlinks[n], "/dev/disk/by-path/"))
        {
          g_ptr_array_add (symlinks_by_path, (gpointer) symlinks[n]);
        }
    }
  g_ptr_array_sort (symlinks_by_id, (GCompareFunc) ptr_str_array_compare);
  g_ptr_array_sort (symlinks_by_path, (GCompareFunc) ptr_str_array_compare);
  g_ptr_array_add (symlinks_by_id, NULL);
  g_ptr_array_add (symlinks_by_path, NULL);
  device_set_device_file_by_id (device, (GStrv) symlinks_by_id->pdata);
  device_set_device_file_by_path (device, (GStrv) symlinks_by_path->pdata);
  g_ptr_array_free (symlinks_by_id, TRUE);
  g_ptr_array_free (symlinks_by_path, TRUE);

  device_set_device_is_removable (device, (sysfs_get_int (device->priv->native_path, "removable") != 0));

  /* device_is_media_available and device_media_detection_time property */
  if (device->priv->device_is_removable)
    {
      media_available = FALSE;

      if (!g_udev_device_get_property_as_boolean (device->priv->d, "ID_CDROM"))
        {
          int fd;
          fd = open (device->priv->device_file, O_RDONLY);
          if (fd >= 0)
            {
              media_available = TRUE;
              close (fd);
            }
        }
      else
        {
          if (g_udev_device_get_property_as_boolean (device->priv->d, "ID_CDROM_MEDIA"))
            {
              media_available = TRUE;
            }
          else
            {
              media_available = FALSE;
            }
        }
    }
  else
    {
      media_available = TRUE;
    }
  device_set_device_is_media_available (device, media_available);
  if (media_available)
    {
      if (device->priv->device_media_detection_time == 0)
        device_set_device_media_detection_time (device, (guint64) time (NULL));
    }
  else
    {
      device_set_device_media_detection_time (device, 0);
    }

  /* device_size, device_block_size and device_is_read_only properties */
  if (device->priv->device_is_media_available)
    {
      guint64 block_size;

      device_set_device_size (device, sysfs_get_uint64 (device->priv->native_path, "size") * ((guint64) 512));
      device_set_device_is_read_only (device, (sysfs_get_int (device->priv->native_path, "ro") != 0));
      /* This is not available on all devices so fall back to 512 if unavailable.
       *
       * Another way to get this information is the BLKSSZGET ioctl but we don't want
       * to open the device. Ideally vol_id would export it.
       */
      block_size = sysfs_get_uint64 (device->priv->native_path, "queue/hw_sector_size");
      if (block_size == 0)
        block_size = 512;
      device_set_device_block_size (device, block_size);

    }
  else
    {
      device_set_device_size (device, 0);
      device_set_device_block_size (device, 0);
      device_set_device_is_read_only (device, FALSE);
    }

  /* Maintain (non-exported) properties holders and slaves for the holders resp. slaves
   * directories in sysfs. The entries in these arrays are object paths - we ignore
   * an entry unless it corresponds to an device in our local database.
   */
  path = g_build_filename (device->priv->native_path, "slaves", NULL);
  slaves = g_ptr_array_new ();
  if ((dir = g_dir_open (path, 0, NULL)) != NULL)
    {
      while ((name = g_dir_read_name (dir)) != NULL)
        {
          Device *device2;

          s = compute_object_path (name);

          device2 = daemon_local_find_by_object_path (device->priv->daemon, s);
          if (device2 != NULL)
            {
              g_ptr_array_add (slaves, s);
            }
          else
            {
              g_free (s);
            }
        }
      g_dir_close (dir);
    }
  g_free (path);
  g_ptr_array_sort (slaves, (GCompareFunc) ptr_str_array_compare);
  g_ptr_array_add (slaves, NULL);
  device_set_slaves_objpath (device, (GStrv) slaves->pdata);
  g_ptr_array_foreach (slaves, (GFunc) g_free, NULL);
  g_ptr_array_free (slaves, TRUE);

  path = g_build_filename (device->priv->native_path, "holders", NULL);
  holders = g_ptr_array_new ();
  if ((dir = g_dir_open (path, 0, NULL)) != NULL)
    {
      while ((name = g_dir_read_name (dir)) != NULL)
        {
          Device *device2;

          s = compute_object_path (name);
          device2 = daemon_local_find_by_object_path (device->priv->daemon, s);
          if (device2 != NULL)
            {
              g_ptr_array_add (holders, s);
            }
          else
            {
              g_free (s);
            }
        }
      g_dir_close (dir);
    }
  g_free (path);
  g_ptr_array_sort (holders, (GCompareFunc) ptr_str_array_compare);
  g_ptr_array_add (holders, NULL);
  device_set_holders_objpath (device, (GStrv) holders->pdata);
  g_ptr_array_foreach (holders, (GFunc) g_free, NULL);
  g_ptr_array_free (holders, TRUE);

  /* ------------------------------------- */
  /* Now set all properties from udev data */
  /* ------------------------------------- */

  /* at this point we have
   *
   *  - device_file
   *  - device_file_by_id
   *  - device_file_by_path
   *  - device_size
   *  - device_block_size
   *  - device_is_removable
   *  - device_is_read_only
   *  - device_is_drive
   *  - device_is_media_available
   *  - device_is_partition
   *  - device_is_partition_table
   *  - slaves_objpath
   *  - holders_objpath
   *
   *  - partition_number
   *  - partition_slave
   *
   */

  /* device_is_linux_loop and linux_loop_* properties */
  if (!update_info_linux_loop (device))
    goto out;

  /* partition_* properties */
  if (!update_info_partition (device))
    goto out;

  /* partition_table_* properties */
  if (!update_info_partition_table (device))
    goto out;

  /* device_presentation_hide, device_presentation_name and device_presentation_icon_name properties */
  if (!update_info_presentation (device))
    goto out;

  /* id_* properties */
  if (!update_info_id (device))
    goto out;

  /* drive_* properties */
  if (!update_info_drive (device))
    goto out;

  /* device_is_optical_disc and optical_disc_* properties */
  if (!update_info_optical_disc (device))
    goto out;

  /* device_is_luks and luks_holder */
  if (!update_info_luks (device))
    goto out;

  /* device_is_luks_cleartext and luks_cleartext_* properties */
  if (!update_info_luks_cleartext (device))
    goto out;

#ifdef HAVE_LVM2
  /* device_is_linux_lvm2_lv and linux_lvm2_lv_* properties */
  if (!update_info_linux_lvm2_lv (device))
    goto out;

  /* device_is_linux_lvm2_pv and linux_lvm2_pv_* properties */
  if (!update_info_linux_lvm2_pv (device))
    goto out;
#endif

#ifdef HAVE_DMMP
  /* device_is_linux_dmmp and linux_dmmp_* properties */
  if (!update_info_linux_dmmp (device))
    goto out;

  /* device_is_partition and partition_* properties for dm-0 "partitions" on a multi-path device  */
  if (!update_info_partition_on_linux_dmmp (device))
    goto out;

  /* device_is_linux_dmmp_component and linux_dmmp_component_* properties */
  if (!update_info_linux_dmmp_component (device))
    goto out;
#endif

  /* device_is_linux_md_component and linux_md_component_* properties */
  if (!update_info_linux_md_component (device))
    goto out;

  /* device_is_linux_md and linux_md_* properties */
  if (!update_info_linux_md (device))
    goto out;

  /* drive_ata_smart_* properties */
  if (!update_info_drive_ata_smart (device))
    goto out;

  /* drive_can_spindown property */
  if (!update_info_drive_can_spindown (device))
    goto out;

  /* device_is_system_internal property */
  if (!update_info_is_system_internal (device))
    goto out;

  /* device_is_mounted, device_mount, device_mounted_by_uid */
  if (!update_info_mount_state (device))
    goto out;

  /* device_is_media_change_detected, device_is_media_change_detection_* properties */
  if (!update_info_media_detection (device))
    goto out;

  /* drive_adapter proprety */
  if (!update_info_drive_adapter (device))
    goto out;

  /* drive_ports property */
  if (!update_info_drive_ports (device))
    goto out;

  /* drive_similar_devices property */
  if (!update_info_drive_similar_devices (device))
    goto out;

  ret = TRUE;

 out:

  /* Now check if holders/ or slaves/ has changed since last update. We compute
   * the delta and do update_info() on each holder/slave that has been
   * added/removed.
   *
   * Note that this won't trigger an endless loop since we look at the diffs.
   *
   * We have to do this because the kernel doesn't generate any 'change' event
   * when slaves/ or holders/ change. This is unfortunate because we *need* such
   * a change event to update properties devices (for example: luks_holder).
   *
   * We do the update in idle because the update may depend on the device
   * currently being processed being added.
   */

  cur_slaves_objpath = dup_list_from_ptrarray (device->priv->slaves_objpath);
  cur_holders_objpath = dup_list_from_ptrarray (device->priv->holders_objpath);

  old_slaves_objpath = g_list_sort (old_slaves_objpath, (GCompareFunc) g_strcmp0);
  old_holders_objpath = g_list_sort (old_holders_objpath, (GCompareFunc) g_strcmp0);
  cur_slaves_objpath = g_list_sort (cur_slaves_objpath, (GCompareFunc) g_strcmp0);
  cur_holders_objpath = g_list_sort (cur_holders_objpath, (GCompareFunc) g_strcmp0);

  diff_sorted_lists (old_slaves_objpath, cur_slaves_objpath, (GCompareFunc) g_strcmp0, &added_objpath, &removed_objpath);
  for (l = added_objpath; l != NULL; l = l->next)
    {
      const gchar *objpath2 = l->data;
      Device *device2;

      device2 = daemon_local_find_by_object_path (device->priv->daemon, objpath2);
      if (device2 != NULL)
        {
          update_info_in_idle (device2);
        }
      else
        {
          g_print ("**** NOTE: %s added non-existant slave %s\n", device->priv->object_path, objpath2);
        }
    }
  for (l = removed_objpath; l != NULL; l = l->next)
    {
      const gchar *objpath2 = l->data;
      Device *device2;

      device2 = daemon_local_find_by_object_path (device->priv->daemon, objpath2);
      if (device2 != NULL)
        {
          update_info_in_idle (device2);
        }
      else
        {
        }
    }
  g_list_free (added_objpath);
  g_list_free (removed_objpath);

  diff_sorted_lists (old_holders_objpath,
                     cur_holders_objpath,
                     (GCompareFunc) g_strcmp0,
                     &added_objpath,
                     &removed_objpath);
  for (l = added_objpath; l != NULL; l = l->next)
    {
      const gchar *objpath2 = l->data;
      Device *device2;

      device2 = daemon_local_find_by_object_path (device->priv->daemon, objpath2);
      if (device2 != NULL)
        {
          update_info_in_idle (device2);
        }
      else
        {
          g_print ("**** NOTE: %s added non-existant holder %s\n", device->priv->object_path, objpath2);
        }
    }
  for (l = removed_objpath; l != NULL; l = l->next)
    {
      const gchar *objpath2 = l->data;
      Device *device2;

      device2 = daemon_local_find_by_object_path (device->priv->daemon, objpath2);
      if (device2 != NULL)
        {
          update_info_in_idle (device2);
        }
      else
        {
        }
    }
  g_list_free (added_objpath);
  g_list_free (removed_objpath);

  g_list_foreach (old_slaves_objpath, (GFunc) g_free, NULL);
  g_list_free (old_slaves_objpath);
  g_list_foreach (old_holders_objpath, (GFunc) g_free, NULL);
  g_list_free (old_holders_objpath);
  g_list_foreach (cur_slaves_objpath, (GFunc) g_free, NULL);
  g_list_free (cur_slaves_objpath);
  g_list_foreach (cur_holders_objpath, (GFunc) g_free, NULL);
  g_list_free (cur_holders_objpath);

  PROFILE ("update_info(device=%s) end", device->priv->native_path);
  return ret;
}

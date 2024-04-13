update_info_linux_md (Device *device)
{
  gboolean ret;
  guint n;
  gchar *s;
  gchar *p;

  ret = FALSE;

  if (sysfs_file_exists (device->priv->native_path, "md"))
    {
      gchar *uuid;
      gint num_raid_devices;
      gchar *raid_level;
      gchar *array_state;
      Device *slave;
      GPtrArray *md_slaves;
      const gchar *md_name;
      const gchar *md_home_host;

      device_set_device_is_linux_md (device, TRUE);

      /* figure out if the array is active */
      array_state = sysfs_get_string (device->priv->native_path, "md/array_state");
      if (array_state == NULL)
        {
          g_print ("**** NOTE: Linux MD array %s has no array_state file'; removing\n", device->priv->native_path);
          goto out;
        }
      g_strstrip (array_state);

      /* ignore clear arrays since these have no devices, no size, no level */
      if (strcmp (array_state, "clear") == 0)
        {
          g_print ("**** NOTE: Linux MD array %s is 'clear'; removing\n", device->priv->native_path);
          g_free (array_state);
          goto out;
        }

      device_set_linux_md_state (device, array_state);
      g_free (array_state);

      /* find a slave from the array */
      slave = NULL;
      for (n = 0; n < device->priv->slaves_objpath->len; n++)
        {
          const gchar *slave_objpath;

          slave_objpath = device->priv->slaves_objpath->pdata[n];
          slave = daemon_local_find_by_object_path (device->priv->daemon, slave_objpath);
          if (slave != NULL)
            break;
        }

      uuid = g_strdup (g_udev_device_get_property (device->priv->d, "MD_UUID"));
      num_raid_devices = sysfs_get_int (device->priv->native_path, "md/raid_disks");
      raid_level = g_strstrip (sysfs_get_string (device->priv->native_path, "md/level"));

      if (slave != NULL)
        {
          /* if the UUID isn't set by the udev rules (array may be inactive) get it from a slave */
          if (uuid == NULL || strlen (uuid) == 0)
            {
              g_free (uuid);
              uuid = g_strdup (slave->priv->linux_md_component_uuid);
            }

          /* ditto for raid level */
          if (raid_level == NULL || strlen (raid_level) == 0)
            {
              g_free (raid_level);
              raid_level = g_strdup (slave->priv->linux_md_component_level);
            }

          /* and num_raid_devices too */
          if (device->priv->linux_md_num_raid_devices == 0)
            {
              num_raid_devices = slave->priv->linux_md_component_num_raid_devices;
            }
        }

      device_set_linux_md_uuid (device, uuid);
      device_set_linux_md_num_raid_devices (device, num_raid_devices);
      device_set_linux_md_level (device, raid_level);
      g_free (raid_level);
      g_free (uuid);

      /* infer the array name and homehost */
      p = g_strdup (g_udev_device_get_property (device->priv->d, "MD_NAME"));
      s = NULL;
      if (p != NULL)
        s = strstr (p, ":");
      if (s != NULL)
        {
          *s = '\0';
          md_home_host = p;
          md_name = s + 1;
        }
      else
        {
          md_home_host = "";
          md_name = p;
        }
      device_set_linux_md_home_host (device, md_home_host);
      device_set_linux_md_name (device, md_name);
      g_free (p);

      s = g_strstrip (sysfs_get_string (device->priv->native_path, "md/metadata_version"));
      device_set_linux_md_version (device, s);
      g_free (s);

      /* Go through all block slaves and build up the linux_md_slaves property
       *
       * Also update the slaves since the slave state may have changed.
       */
      md_slaves = g_ptr_array_new ();
      for (n = 0; n < device->priv->slaves_objpath->len; n++)
        {
          Device *slave_device;
          const gchar *slave_objpath;

          slave_objpath = device->priv->slaves_objpath->pdata[n];
          g_ptr_array_add (md_slaves, (gpointer) slave_objpath);
          slave_device = daemon_local_find_by_object_path (device->priv->daemon, slave_objpath);
          if (slave_device != NULL)
            {
              update_info (slave_device);
            }
        }
      g_ptr_array_sort (md_slaves, (GCompareFunc) ptr_str_array_compare);
      g_ptr_array_add (md_slaves, NULL);
      device_set_linux_md_slaves (device, (GStrv) md_slaves->pdata);
      g_ptr_array_free (md_slaves, TRUE);

      /* TODO: may race */
      device_set_drive_vendor (device, "Linux");
      if (device->priv->linux_md_level != NULL)
        s = g_strdup_printf ("Software RAID %s", device->priv->linux_md_level);
      else
        s = g_strdup_printf ("Software RAID");
      device_set_drive_model (device, s);
      g_free (s);
      device_set_drive_revision (device, device->priv->linux_md_version);
      device_set_drive_connection_interface (device, "virtual");
      device_set_drive_serial (device, device->priv->linux_md_uuid);

      /* RAID-0 can never resync or run degraded */
      if (g_strcmp0 (device->priv->linux_md_level, "raid0") == 0 || g_strcmp0 (device->priv->linux_md_level, "linear")
          == 0)
        {
          device_set_linux_md_sync_action (device, "idle");
          device_set_linux_md_is_degraded (device, FALSE);
        }
      else
        {
          gchar *degraded_file;
          gint num_degraded_devices;

          degraded_file = sysfs_get_string (device->priv->native_path, "md/degraded");
          if (degraded_file == NULL)
            {
              num_degraded_devices = 0;
            }
          else
            {
              num_degraded_devices = strtol (degraded_file, NULL, 0);
            }
          g_free (degraded_file);

          device_set_linux_md_is_degraded (device, (num_degraded_devices > 0));

          s = g_strstrip (sysfs_get_string (device->priv->native_path, "md/sync_action"));
          device_set_linux_md_sync_action (device, s);
          g_free (s);

          if (device->priv->linux_md_sync_action == NULL || strlen (device->priv->linux_md_sync_action) == 0)
            {
              device_set_linux_md_sync_action (device, "idle");
            }

          /* if not idle; update percentage and speed */
          if (g_strcmp0 (device->priv->linux_md_sync_action, "idle") != 0)
            {
              char *s;
              guint64 done;
              guint64 remaining;

              s = g_strstrip (sysfs_get_string (device->priv->native_path, "md/sync_completed"));
              if (sscanf (s, "%" G_GUINT64_FORMAT " / %" G_GUINT64_FORMAT "", &done, &remaining) == 2)
                {
                  device_set_linux_md_sync_percentage (device, 100.0 * ((double) done) / ((double) remaining));
                }
              else
                {
                  g_warning ("cannot parse md/sync_completed for %s: '%s'", device->priv->native_path, s);
                }
              g_free (s);

              device_set_linux_md_sync_speed (device, 1000L * sysfs_get_uint64 (device->priv->native_path,
                                                                                "md/sync_speed"));

              /* Since the kernel doesn't emit uevents while the job is pending, set up
               * a timeout for every two seconds to synthesize the change event so we can
               * refresh the completed/speed properties.
               */
              if (device->priv->linux_md_poll_timeout_id == 0)
                {
                  device->priv->linux_md_poll_timeout_id = g_timeout_add_seconds_full (G_PRIORITY_DEFAULT,
                                                                                       2,
                                                                                       poll_syncing_md_device,
                                                                                       g_object_ref (device),
                                                                                       g_object_unref);
                }
            }
          else
            {
              device_set_linux_md_sync_percentage (device, 0.0);
              device_set_linux_md_sync_speed (device, 0);
            }
        }

    }
  else
    {
      device_set_device_is_linux_md (device, FALSE);
      device_set_linux_md_state (device, NULL);
      device_set_linux_md_level (device, NULL);
      device_set_linux_md_num_raid_devices (device, 0);
      device_set_linux_md_uuid (device, NULL);
      device_set_linux_md_home_host (device, NULL);
      device_set_linux_md_name (device, NULL);
      device_set_linux_md_version (device, NULL);
      device_set_linux_md_slaves (device, NULL);
      device_set_linux_md_is_degraded (device, FALSE);
      device_set_linux_md_sync_action (device, NULL);
      device_set_linux_md_sync_percentage (device, 0.0);
      device_set_linux_md_sync_speed (device, 0);
    }

  ret = TRUE;

 out:
  return ret;
}

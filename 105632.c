update_info_linux_md_component (Device *device)
{
  if (g_strcmp0 (device->priv->id_type, "linux_raid_member") == 0)
    {
      const gchar *md_comp_level;
      gint md_comp_num_raid_devices;
      const gchar *md_comp_uuid;
      const gchar *md_comp_home_host;
      const gchar *md_comp_name;
      const gchar *md_comp_version;
      gchar *md_name;
      gchar *s;
      int md_comp_position;

      md_comp_position = -1;

      device_set_device_is_linux_md_component (device, TRUE);

      /* linux_md_component_holder and linux_md_component_state */
      if (device->priv->holders_objpath->len == 1)
        {
          Device *holder;
          gchar **state_tokens;

          device_set_linux_md_component_holder (device, device->priv->holders_objpath->pdata[0]);
          state_tokens = NULL;
          holder = daemon_local_find_by_object_path (device->priv->daemon, device->priv->holders_objpath->pdata[0]);
          if (holder != NULL && holder->priv->device_is_linux_md)
            {
              gchar *dev_name;
              gchar *md_dev_path;
              gchar *state_contents;
              gchar *slot_contents;
              gint slot_number;
              gchar *endp;

              dev_name = g_path_get_basename (device->priv->native_path);
              md_dev_path = g_strdup_printf ("%s/md/dev-%s", holder->priv->native_path, dev_name);
              state_contents = sysfs_get_string (md_dev_path, "state");
              g_strstrip (state_contents);
              state_tokens = g_strsplit (state_contents, ",", 0);

              slot_contents = sysfs_get_string (md_dev_path, "slot");
              g_strstrip (slot_contents);
              slot_number = strtol (slot_contents, &endp, 0);
              if (endp != NULL && *endp == '\0')
                {
                  md_comp_position = slot_number;
                }

              g_free (slot_contents);
              g_free (state_contents);
              g_free (md_dev_path);
              g_free (dev_name);
            }

          device_set_linux_md_component_state (device, state_tokens);
          g_strfreev (state_tokens);

        }
      else
        {
          /* no holder, nullify properties */
          device_set_linux_md_component_holder (device, NULL);
          device_set_linux_md_component_state (device, NULL);
        }

      md_comp_level = g_udev_device_get_property (device->priv->d, "MD_LEVEL");
      md_comp_num_raid_devices = g_udev_device_get_property_as_int (device->priv->d, "MD_DEVICES");
      md_comp_uuid = g_udev_device_get_property (device->priv->d, "MD_UUID");
      md_name = g_strdup (g_udev_device_get_property (device->priv->d, "MD_NAME"));
      s = NULL;
      if (md_name != NULL)
        s = strstr (md_name, ":");
      if (s != NULL)
        {
          *s = '\0';
          md_comp_home_host = md_name;
          md_comp_name = s + 1;
        }
      else
        {
          md_comp_home_host = "";
          md_comp_name = md_name;
        }
      md_comp_version = device->priv->id_version;

      device_set_linux_md_component_level (device, md_comp_level);
      device_set_linux_md_component_position (device, md_comp_position);
      device_set_linux_md_component_num_raid_devices (device, md_comp_num_raid_devices);
      device_set_linux_md_component_uuid (device, md_comp_uuid);
      device_set_linux_md_component_home_host (device, md_comp_home_host);
      device_set_linux_md_component_name (device, md_comp_name);
      device_set_linux_md_component_version (device, md_comp_version);

      g_free (md_name);
    }
  else
    {
      device_set_device_is_linux_md_component (device, FALSE);
      device_set_linux_md_component_level (device, NULL);
      device_set_linux_md_component_position (device, -1);
      device_set_linux_md_component_num_raid_devices (device, 0);
      device_set_linux_md_component_uuid (device, NULL);
      device_set_linux_md_component_home_host (device, NULL);
      device_set_linux_md_component_name (device, NULL);
      device_set_linux_md_component_version (device, NULL);
      device_set_linux_md_component_holder (device, NULL);
      device_set_linux_md_component_state (device, NULL);
    }

  return TRUE;
}

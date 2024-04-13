update_info_optical_disc (Device *device)
{
  const gchar *cdrom_disc_state;
  gint cdrom_track_count;
  gint cdrom_track_count_audio;
  gint cdrom_session_count;

  /* device_is_optical_disc and optical_disc_* */
  if (g_udev_device_has_property (device->priv->d, "ID_CDROM_MEDIA"))
    {
      device_set_device_is_optical_disc (device, TRUE);

      cdrom_track_count = 0;
      cdrom_track_count_audio = 0;
      cdrom_session_count = 0;

      if (g_udev_device_has_property (device->priv->d, "ID_CDROM_MEDIA_TRACK_COUNT"))
        cdrom_track_count = g_udev_device_get_property_as_int (device->priv->d, "ID_CDROM_MEDIA_TRACK_COUNT");
      if (g_udev_device_has_property (device->priv->d, "ID_CDROM_MEDIA_TRACK_COUNT_AUDIO"))
        cdrom_track_count_audio = g_udev_device_get_property_as_int (device->priv->d,
                                                                     "ID_CDROM_MEDIA_TRACK_COUNT_AUDIO");
      if (g_udev_device_has_property (device->priv->d, "ID_CDROM_MEDIA_SESSION_COUNT"))
        cdrom_session_count = g_udev_device_get_property_as_int (device->priv->d, "ID_CDROM_MEDIA_SESSION_COUNT");
      device_set_optical_disc_num_tracks (device, cdrom_track_count);
      device_set_optical_disc_num_audio_tracks (device, cdrom_track_count_audio);
      device_set_optical_disc_num_sessions (device, cdrom_session_count);
      cdrom_disc_state = g_udev_device_get_property (device->priv->d, "ID_CDROM_MEDIA_STATE");
      device_set_optical_disc_is_blank (device, g_strcmp0 (cdrom_disc_state, "blank") == 0);
      device_set_optical_disc_is_appendable (device, g_strcmp0 (cdrom_disc_state, "appendable") == 0);
      device_set_optical_disc_is_closed (device, g_strcmp0 (cdrom_disc_state, "complete") == 0);
    }
  else
    {
      device_set_device_is_optical_disc (device, FALSE);

      device_set_optical_disc_num_tracks (device, 0);
      device_set_optical_disc_num_audio_tracks (device, 0);
      device_set_optical_disc_num_sessions (device, 0);
      device_set_optical_disc_is_blank (device, FALSE);
      device_set_optical_disc_is_appendable (device, FALSE);
      device_set_optical_disc_is_closed (device, FALSE);
    }

  return TRUE;
}

unlock_cd_tray (Device *device)
{
  /* Unlock CD tray to keep the hardware eject button working */
  if (g_udev_device_has_property (device->priv->d, "ID_CDROM"))
    {
      gint fd;

      g_print ("**** Unlocking CD-ROM door for %s\n", device->priv->device_file);
      fd = open (device->priv->device_file, O_RDONLY);
      if (fd > 0)
        {
          if (ioctl (fd, CDROM_LOCKDOOR, 0) != 0)
            g_warning ("Could not unlock CD-ROM door: %s", strerror (errno));
          close (fd);
        }
      else
        {
          g_warning ("Could not open CD-ROM device: %s", strerror (errno));
        }
    }
}

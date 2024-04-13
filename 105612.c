update_drive_properties_from_sysfs (Device *device)
{
  char *s;
  char *p;
  char *q;
  char *model;
  char *vendor;
  char *subsystem;
  char *serial;
  char *revision;
  const char *connection_interface;
  guint64 connection_speed;

  connection_interface = NULL;
  connection_speed = 0;

  /* walk up the device tree to figure out the subsystem */
  s = g_strdup (device->priv->native_path);
  do
    {
      p = sysfs_resolve_link (s, "subsystem");
      if (p != NULL)
        {
          subsystem = g_path_get_basename (p);
          g_free (p);

          if (strcmp (subsystem, "scsi") == 0)
            {
              connection_interface = "scsi";
              connection_speed = 0;

              /* continue walking up the chain; we just use scsi as a fallback */

              /* grab the names from SCSI since the names from udev currently
               *  - replaces whitespace with _
               *  - is missing for e.g. Firewire
               */
              vendor = sysfs_get_string (s, "vendor");
              if (vendor != NULL)
                {
                  g_strstrip (vendor);
                  /* Don't overwrite what we set earlier from ID_VENDOR */
                  if (device->priv->drive_vendor == NULL)
                    {
                      q = _dupv8 (vendor);
                      device_set_drive_vendor (device, q);
                      g_free (q);
                    }
                  g_free (vendor);
                }

              model = sysfs_get_string (s, "model");
              if (model != NULL)
                {
                  g_strstrip (model);
                  /* Don't overwrite what we set earlier from ID_MODEL */
                  if (device->priv->drive_model == NULL)
                    {
                      q = _dupv8 (model);
                      device_set_drive_model (device, q);
                      g_free (q);
                    }
                  g_free (model);
                }

              /* TODO: need to improve this code; we probably need the kernel to export more
               *       information before we can properly get the type and speed.
               */

              if (device->priv->drive_vendor != NULL && strcmp (device->priv->drive_vendor, "ATA") == 0)
                {
                  connection_interface = "ata";
                  break;
                }

            }
          else if (strcmp (subsystem, "usb") == 0)
            {
              double usb_speed;

              /* both the interface and the device will be 'usb'. However only
               * the device will have the 'speed' property.
               */
              usb_speed = sysfs_get_double (s, "speed");
              if (usb_speed > 0)
                {
                  connection_interface = "usb";
                  connection_speed = usb_speed * (1000 * 1000);
                  break;

                }
            }
          else if (strcmp (subsystem, "firewire") == 0 || strcmp (subsystem, "ieee1394") == 0)
            {

              /* TODO: krh has promised a speed file in sysfs; theoretically, the speed can
               *       be anything from 100, 200, 400, 800 and 3200. Till then we just hardcode
               *       a resonable default of 400 Mbit/s.
               */

              connection_interface = "firewire";
              connection_speed = 400 * (1000 * 1000);
              break;

            }
          else if (strcmp (subsystem, "mmc") == 0)
            {

              /* TODO: what about non-SD, e.g. MMC? Is that another bus? */
              connection_interface = "sdio";

              /* Set vendor name. According to this MMC document
               *
               * http://www.mmca.org/membership/IAA_Agreement_10_12_06.pdf
               *
               *  - manfid: the manufacturer id
               *  - oemid: the customer of the manufacturer
               *
               * Apparently these numbers are kept secret. It would be nice
               * to map these into names for setting the manufacturer of the drive,
               * e.g. Panasonic, Sandisk etc.
               */

              model = sysfs_get_string (s, "name");
              if (model != NULL)
                {
                  g_strstrip (model);
                  /* Don't overwrite what we set earlier from ID_MODEL */
                  if (device->priv->drive_model == NULL)
                    {
                      q = _dupv8 (model);
                      device_set_drive_model (device, q);
                      g_free (q);
                    }
                  g_free (model);
                }

              serial = sysfs_get_string (s, "serial");
              if (serial != NULL)
                {
                  g_strstrip (serial);
                  /* Don't overwrite what we set earlier from ID_SERIAL */
                  if (device->priv->drive_serial == NULL)
                    {
                      /* this is formatted as a hexnumber; drop the leading 0x */
                      q = _dupv8 (serial + 2);
                      device_set_drive_serial (device, q);
                      g_free (q);
                    }
                  g_free (serial);
                }

              /* TODO: use hwrev and fwrev files? */
              revision = sysfs_get_string (s, "date");
              if (revision != NULL)
                {
                  g_strstrip (revision);
                  /* Don't overwrite what we set earlier from ID_REVISION */
                  if (device->priv->drive_revision == NULL)
                    {
                      q = _dupv8 (revision);
                      device_set_drive_revision (device, q);
                      g_free (q);
                    }
                  g_free (revision);
                }

              /* TODO: interface speed; the kernel driver knows; would be nice
               * if it could export it */

            }
          else if (strcmp (subsystem, "platform") == 0)
            {
              const gchar *sysfs_name;

              sysfs_name = g_strrstr (s, "/");
              if (g_str_has_prefix (sysfs_name + 1, "floppy."))
                {
                  device_set_drive_vendor (device, "Floppy Drive");
                  connection_interface = "platform";
                }
            }

          g_free (subsystem);
        }

      /* advance up the chain */
      p = g_strrstr (s, "/");
      if (p == NULL)
        break;
      *p = '\0';

      /* but stop at the root */
      if (strcmp (s, "/sys/devices") == 0)
        break;

    }
  while (TRUE);

  if (connection_interface != NULL)
    {
      device_set_drive_connection_interface (device, connection_interface);
      device_set_drive_connection_speed (device, connection_speed);
    }

  g_free (s);
}

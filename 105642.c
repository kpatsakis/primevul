get_udev_device_from_device_file (struct udev *udev,
             const gchar *device_file)
{
  struct udev_device *device;
  struct stat statbuf;

  device = NULL;

  if (stat (device_file, &statbuf) != 0)
    {
      g_printerr ("Error statting %s: %m\n", device_file);
      goto out;
    }

  device = udev_device_new_from_devnum (udev, 'b', statbuf.st_rdev);
  if (device == NULL)
    {
      g_printerr ("Error getting udev device for %s: %m\n", device_file);
      goto out;
    }

 out:
  return device;
}

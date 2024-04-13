filesystem_create_wait_for_cleartext_device_hook (DBusGMethodInvocation *context,
                                                  Device *device,
                                                  gpointer user_data)
{
  MkfsLuksData *data = user_data;

  if (device == NULL)
    {
      /* Dang, unlocking failed. The unlock method have already thrown an exception for us. */
    }
  else
    {
      /* We're unlocked.. awesome.. Now we can _finally_ create the file system.
       * What a ride. We're returning to exactly to where we came from. Back to
       * the source. Only the device is different.
       */

      device_filesystem_create_internal (device,
                                         data->fstype,
                                         data->options,
                                         data->mkfs_hook_func,
                                         data->mkfs_hook_user_data,
                                         data->context);
      mkfse_data_unref (data);
    }
}

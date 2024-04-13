force_luks_teardown (Device *device,
                     Device *cleartext_device,
                     ForceRemovalCompleteFunc callback,
                     gpointer user_data)
{
  /* first we gotta force remove the clear text device */
  force_removal (cleartext_device,
                 force_luks_teardown_cleartext_done,
                 force_luks_teardown_data_new (device, cleartext_device->priv->dm_name, callback, user_data));
}

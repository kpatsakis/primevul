force_unmount_data_unref (ForceUnmountData *data)
{
  g_free (data->mount_path);
  g_free (data);
}

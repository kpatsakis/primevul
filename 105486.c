filesystem_mount_data_free (MountData *data)
{
  g_free (data->mount_point);
  g_free (data);
}

filesystem_mount_data_new (const char *mount_point,
                           gboolean remove_dir_on_unmount)
{
  MountData *data;
  data = g_new0 (MountData, 1);
  data->mount_point = g_strdup (mount_point);
  data->remove_dir_on_unmount = remove_dir_on_unmount;
  return data;
}

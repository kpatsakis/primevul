partition_table_create_data_new (DBusGMethodInvocation *context,
                                 Device *device,
                                 const char *scheme)
{
  CreatePartitionTableData *data;

  data = g_new0 (CreatePartitionTableData, 1);
  data->refcount = 1;

  data->context = context;
  data->device = g_object_ref (device);
  data->scheme = g_strdup (scheme);

  return data;
}

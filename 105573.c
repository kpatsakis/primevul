partition_create_data_new (DBusGMethodInvocation *context,
                           Device *device,
                           guint64 offset,
                           guint64 size,
                           const char *fstype,
                           char **fsoptions)
{
  CreatePartitionData *data;

  data = g_new0 (CreatePartitionData, 1);
  data->refcount = 1;

  data->context = context;
  data->device = g_object_ref (device);
  data->offset = offset;
  data->size = size;
  data->fstype = g_strdup (fstype);
  data->fsoptions = g_strdupv (fsoptions);

  return data;
}

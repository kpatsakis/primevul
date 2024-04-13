partition_modify_data_new (DBusGMethodInvocation *context,
                           Device *device,
                           Device *enclosing_device,
                           const char *type,
                           const char *label,
                           char **flags)
{
  ModifyPartitionData *data;

  data = g_new0 (ModifyPartitionData, 1);

  data->context = context;
  data->device = g_object_ref (device);
  data->enclosing_device = g_object_ref (enclosing_device);
  data->type = g_strdup (type);
  data->label = g_strdup (label);
  data->flags = g_strdupv (flags);

  return data;
}

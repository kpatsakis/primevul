remove_component_data_new (DBusGMethodInvocation *context,
                           Device *slave,
                           char **options)
{
  RemoveComponentData *data;

  data = g_new0 (RemoveComponentData, 1);
  data->refcount = 1;

  data->context = context;
  data->slave = g_object_ref (slave);
  data->options = g_strdupv (options);
  return data;
}

linux_md_create_data_new (DBusGMethodInvocation *context,
                          Daemon *daemon,
                          const char *first_component_objpath)
{
  LinuxMdCreateData *data;

  data = g_new0 (LinuxMdCreateData, 1);
  data->refcount = 1;

  data->context = context;
  data->daemon = g_object_ref (daemon);
  data->first_component_objpath = g_strdup (first_component_objpath);
  return data;
}

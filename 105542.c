linux_md_start_data_new (DBusGMethodInvocation *context,
                         Daemon *daemon,
                         const char *uuid)
{
  LinuxMdStartData *data;

  data = g_new0 (LinuxMdStartData, 1);
  data->refcount = 1;

  data->context = context;
  data->daemon = g_object_ref (daemon);
  data->uuid = g_strdup (uuid);
  return data;
}

lvm2_lv_create_data_new (DBusGMethodInvocation *context,
                         Daemon *daemon,
                         const gchar *vg_uuid,
                         const gchar *lv_name,
                         const char *fstype,
                         char **fsoptions)
{
  CreateLvm2LVData *data;

  data = g_new0 (CreateLvm2LVData, 1);
  data->refcount = 1;

  data->context = context;
  data->daemon = g_object_ref (daemon);
  data->vg_uuid = g_strdup (vg_uuid);
  data->lv_name = g_strdup (lv_name);
  data->fstype = g_strdup (fstype);
  data->fsoptions = g_strdupv (fsoptions);

  return data;
}

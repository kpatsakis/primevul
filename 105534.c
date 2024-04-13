linux_md_create_data_unref (LinuxMdCreateData *data)
{
  data->refcount--;
  if (data->refcount == 0)
    {
      g_object_unref (data->daemon);
      g_free (data->first_component_objpath);
      g_free (data);
    }
}

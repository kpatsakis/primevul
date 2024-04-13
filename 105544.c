linux_md_start_data_unref (LinuxMdStartData *data)
{
  data->refcount--;
  if (data->refcount == 0)
    {
      g_object_unref (data->daemon);
      g_free (data->uuid);
      g_free (data);
    }
}

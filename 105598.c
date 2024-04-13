remove_component_data_unref (RemoveComponentData *data)
{
  data->refcount--;
  if (data->refcount == 0)
    {
      g_object_unref (data->slave);
      g_free (data->options);
      g_free (data);
    }
}

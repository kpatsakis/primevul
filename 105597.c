remove_component_data_ref (RemoveComponentData *data)
{
  data->refcount++;
  return data;
}

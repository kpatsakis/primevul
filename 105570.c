mkfse_data_ref (MkfsLuksData *data)
{
  data->refcount++;
  return data;
}

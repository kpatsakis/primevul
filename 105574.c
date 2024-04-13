partition_create_data_ref (CreatePartitionData *data)
{
  data->refcount++;
  return data;
}

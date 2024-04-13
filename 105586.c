partition_table_create_data_ref (CreatePartitionTableData *data)
{
  data->refcount++;
  return data;
}
